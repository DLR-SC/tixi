/*
* Copyright (C) 2017 German Aerospace Center (DLR/SC)
*
* Created: 2017-06-30 Jan Kleinert <Jan.Kleinert@dlr.de>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "test.h" // Brings in the GTest framework
#include "tixicpp.h"

#define EXPECT_TIXI_ERROR(RC, cmd)          \
    ASSERT_THROW(cmd, tixi::TixiError);     \
    try {                                   \
        cmd;                                \
    }                                       \
    catch (tixi::TixiError const & err) {   \
        EXPECT_EQ(RC, err.returnCode());    \
    }

static TixiDocumentHandle documentHandle = -1;

TEST(CppWrapper, createDocumentInvalidRootNodeName)
{
    std::string rootElementName = "1";
    EXPECT_TIXI_ERROR( INVALID_XML_NAME, documentHandle =  tixi::TixiCreateDocument( rootElementName ) );
}

TEST(CppWrapper, createDocumentAddCPACSHeader)
{
  std::string rootElementName = "rootElement";
  std::string cpacsName = "cpacsName";
  std::string cpacsCreator = "cpacsCreator";
  std::string      version = "cpacsVersion";
  std::string cpacsVersion = "2.0";
  std::string cpacsDescription = "cpacsDescription";
  std::string text;

  ASSERT_NO_THROW( documentHandle = tixi::TixiCreateDocument( rootElementName ) );
  ASSERT_NO_THROW( tixi::TixiAddCpacsHeader( documentHandle, cpacsName, cpacsCreator, version, cpacsDescription, cpacsVersion ) );

  // Now check the different header values
  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/name" ) );
  EXPECT_STREQ(cpacsName.c_str(), text.c_str());

  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/creator") );
  EXPECT_STREQ(cpacsCreator.c_str(), text.c_str());

  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/version") );
  EXPECT_STREQ(version.c_str(), text.c_str());

  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/cpacsVersion") );
  EXPECT_STREQ(cpacsVersion.c_str(), text.c_str());

  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/description") );
  EXPECT_STREQ(cpacsDescription.c_str(), text.c_str());
}

TEST(CppWrapper, importFromString)
{
    std::string s = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root/>";
    TixiDocumentHandle handle;
    ASSERT_NO_THROW( handle = tixi::TixiImportFromString(s) );
}

TEST(CppWrapper, exportAsString)
{
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument(xmlFilename) );

    std::string text;
    std::string text2;
    ASSERT_NO_THROW( text=tixi::TixiExportDocumentAsString(documentHandle) );
    EXPECT_TIXI_ERROR( INVALID_HANDLE, text2=tixi::TixiExportDocumentAsString(-1) );
}

TEST(CppWrapper, getAttributeNames)
{
    TixiDocumentHandle documentHandle;
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument(xmlFilename) );

    std::vector<std::string> attributeNames;
    ASSERT_NO_THROW( attributeNames=tixi::TixiGetAttributeNames(documentHandle, "/plane/aPoint[1]") );
    EXPECT_EQ( 2,        attributeNames.size());
    EXPECT_EQ( "system", attributeNames[0]);
    EXPECT_EQ( "type",   attributeNames[1]);
}

TEST(CppWrapper, getAttribute)
{
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument(xmlFilename) );


    std::string text;
    double numberd = 0.;
    int numberi = 0;
    bool mybool = false;


    EXPECT_TIXI_ERROR( INVALID_HANDLE,          text = tixi::TixiGetTextAttribute(-1, "plane/name", "non_existing_attribute") );
    EXPECT_TIXI_ERROR( ATTRIBUTE_NOT_FOUND,     text = tixi::TixiGetAttribute<std::string>(documentHandle, "/plane/wings/wing[1]", "non_existing_attribute") );
    EXPECT_TIXI_ERROR( ELEMENT_PATH_NOT_UNIQUE, text = tixi::TixiGetAttribute<std::string>(documentHandle, "/plane/wings/wing", "position") );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi::TixiGetAttribute<std::string>( documentHandle,  "/plane/wings", "numberOfWings" ) );
    EXPECT_EQ( "2", text);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberd=tixi::TixiGetAttribute<double>( documentHandle, "/plane/coordinateOrigin", "scaling" ) );
    EXPECT_NEAR(1.3456, numberd, 1e-10);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberi=tixi::TixiGetAttribute<int>( documentHandle,  "/plane/wings", "numberOfWings" ) );
    EXPECT_EQ( 2, numberi);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( mybool=tixi::TixiGetAttribute<bool>( documentHandle, "/plane/wings/wing[1]", "top" ) );
    ASSERT_TRUE(mybool);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( mybool=tixi::TixiGetAttribute<bool>( documentHandle, "/plane/wings/wing[2]", "top" ) );
    ASSERT_FALSE(mybool);
}

TEST(CppWrapper, checkRemoveAttribute){
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument("TestData/in.xml") );
    EXPECT_TRUE(tixi::TixiCheckAttribute( documentHandle, "/plane/wings", "numberOfWings") );
    ASSERT_NO_THROW( tixi::TixiRemoveAttribute( documentHandle, "/plane/wings", "numberOfWings" ) );
    EXPECT_FALSE(tixi::TixiCheckAttribute( documentHandle, "/plane/wings", "numberOfWings"));
}

TEST(CppWrapper, getElement)
{
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument("TestData/in.xml") );

    std::string text;
    double numberd;
    int numberi;
    bool myBool = false;

    /*--------------------------------------------------------------------*/
    EXPECT_TIXI_ERROR( INVALID_HANDLE,          text=tixi::TixiGetElement<std::string>( -1            , "/plane/name/pp"       ) );
    EXPECT_TIXI_ERROR( ELEMENT_NOT_FOUND,       text=tixi::TixiGetElement<std::string>( documentHandle, "/plane/name/pp"       ) );
    EXPECT_TIXI_ERROR( INVALID_XPATH,           text=tixi::TixiGetElement<std::string>( documentHandle, "cc/plane/na|<<me/pp"  ) );
    EXPECT_TIXI_ERROR( ELEMENT_PATH_NOT_UNIQUE, text=tixi::TixiGetElement<std::string>( documentHandle, "/plane/wings/wing"    ) );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi::TixiGetElement<std::string>( documentHandle, "/plane/empty" ) );
    EXPECT_STREQ( "", text.c_str() );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi::TixiGetElement<std::string>( documentHandle, "/plane/name" ) );
    EXPECT_STREQ( "Junkers JU 52", text.c_str() );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberd=tixi::TixiGetElement<double>( documentHandle, "/plane/wings/wing[1]/centerOfGravity/x" ) );
    EXPECT_NEAR( 30.0, numberd, 1e-10 );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberi=tixi::TixiGetElement<int>( documentHandle, "/plane/numberOfPassengers" ) );
    EXPECT_EQ( 57, numberi );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( myBool=tixi::TixiGetElement<bool>( documentHandle, "/plane/bool/aBool1" ) );
    EXPECT_TRUE( myBool );
}

TEST(CppWrapper, checkRemoveElement){
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument("TestData/in.xml") );
    EXPECT_TRUE( tixi::TixiCheckElement( documentHandle, "/plane/aPoint/point/z") );
    ASSERT_NO_THROW( tixi::TixiRemoveElement( documentHandle, "/plane/aPoint/point/z" ) );
    EXPECT_FALSE( tixi::TixiCheckElement( documentHandle, "/plane/aPoint/point/z") );
}

TEST(CppWrapper, saveAttribute){

    std::string rootElementName = "rootElement";
    ASSERT_NO_THROW( documentHandle = tixi::TixiCreateDocument( rootElementName ) );
    ASSERT_NO_THROW( tixi::TixiCreateElement(documentHandle, "/rootElement/element") );

    ASSERT_NO_THROW( tixi::TixiSaveAttribute( documentHandle, "/rootElement/element", "sattribute", "stringAttribute") );
    ASSERT_NO_THROW( tixi::TixiSaveAttribute( documentHandle, "/rootElement/element", "dattribute", 3.14159) );
    ASSERT_NO_THROW( tixi::TixiSaveAttribute( documentHandle, "/rootElement/element", "iattribute", 4711) );
    ASSERT_NO_THROW( tixi::TixiSaveAttribute( documentHandle, "/rootElement/element", "battribute", true) );
}

TEST(CppWrapper, saveElement){

    const std::string rootElementName = "rootElement";
    ASSERT_NO_THROW( documentHandle = tixi::TixiCreateDocument(rootElementName) );

    ASSERT_NO_THROW( tixi::TixiSaveElement(documentHandle, "/" + rootElementName + "/cselement", "cstringElement") );
    ASSERT_NO_THROW( tixi::TixiSaveElement(documentHandle, "/" + rootElementName + "/selement", std::string("stringElement")) );
    ASSERT_NO_THROW( tixi::TixiSaveElement(documentHandle, "/" + rootElementName + "/delement", 3.14159) );
    ASSERT_NO_THROW( tixi::TixiSaveElement(documentHandle, "/" + rootElementName + "/ielement", 4711) );
    ASSERT_NO_THROW( tixi::TixiSaveElement(documentHandle, "/" + rootElementName + "/belement", true) );
}

TEST(CppWrapper, getNamedChildrenCount){
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument("TestData/in.xml") );

    int count=-1;

    ASSERT_NO_THROW( count = tixi::TixiGetNamedChildrenCount( documentHandle, "/plane/wings/this_child_does_not_exist") );
    EXPECT_EQ( 0, count );

    ASSERT_NO_THROW( count = tixi::TixiGetNamedChildrenCount( documentHandle, "/plane/wings") );
    EXPECT_EQ( 1, count );

    ASSERT_NO_THROW( count = tixi::TixiGetNamedChildrenCount( documentHandle, "/plane/wings/wing") );
    EXPECT_EQ( 2, count );
}

TEST(CppWrapper, getChildElementPaths) {
    ASSERT_NO_THROW(documentHandle = tixi::TixiOpenDocument("TestData/in.xml"));

    std::vector<std::string> childPaths;
    ASSERT_NO_THROW(childPaths = tixi::TixiGetChildElementPaths(documentHandle, "/plane/wings"));
    EXPECT_EQ(2, childPaths.size());
    EXPECT_EQ("/plane/wings/wing[1]", childPaths[0]);
    EXPECT_EQ("/plane/wings/wing[2]", childPaths[1]);

    ASSERT_NO_THROW(childPaths = tixi::TixiGetChildElementPaths(documentHandle, "/plane/coordinateOrigin"));
    EXPECT_EQ(3, childPaths.size());
    EXPECT_EQ("/plane/coordinateOrigin/x[1]", childPaths[0]);
    EXPECT_EQ("/plane/coordinateOrigin/y[1]", childPaths[1]);
    EXPECT_EQ("/plane/coordinateOrigin/z[1]", childPaths[2]);

    ASSERT_NO_THROW(childPaths = tixi::TixiGetChildElementPaths(documentHandle, "/plane/coordinateOrigin/x"));
    EXPECT_EQ(0, childPaths.size());
}

