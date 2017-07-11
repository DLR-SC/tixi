/*
* Copyright (C) 2017 German Aerospace Center (DLR/SC)
*
* Created: 2017-06-30 Jan Kkleinert <Jan.Kleinert@dlr.de>
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

#define EXPECT_TIXI_ERROR(cmd,RC)           \
    ASSERT_THROW(cmd,tixi::TixiError);     \
    try {                                   \
        cmd;                                \
    }                                       \
    catch(tixi::TixiError const & err) {   \
        EXPECT_EQ(err.returnCode(),RC);     \
    }

static TixiDocumentHandle documentHandle = -1;

TEST(CppWrapper, createDocumentInvalidRootNodeName)
{
    std::string rootElementName = "1";
    EXPECT_TIXI_ERROR( documentHandle =  tixi::TixiCreateDocument( rootElementName ),INVALID_XML_NAME );
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
  EXPECT_EQ(text, cpacsName);

  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/creator") );
  EXPECT_EQ(text, cpacsCreator);

  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/version") );
  EXPECT_EQ(text, version);

  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/cpacsVersion") );
  EXPECT_EQ(text, cpacsVersion);

  ASSERT_NO_THROW( text = tixi::TixiGetTextElement( documentHandle, "/rootElement/header/description") );
  EXPECT_EQ(text, cpacsDescription);
}

TEST(CppWrapper,importFromString)
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
    EXPECT_TIXI_ERROR( text2=tixi::TixiExportDocumentAsString(-1),INVALID_HANDLE );
}

TEST(CppWrapper,getAttributeNames)
{
    TixiDocumentHandle documentHandle;
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument(xmlFilename) );

    std::vector<std::string> attributeNames;
    ASSERT_NO_THROW( attributeNames=tixi::TixiGetAttributeNames(documentHandle,"/plane/aPoint[1]") );
    EXPECT_EQ( attributeNames.size(),2);
    EXPECT_EQ( attributeNames[0],"system");
    EXPECT_EQ( attributeNames[1],"type");
}

TEST(CppWrapper,getAttribute)
{
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument(xmlFilename) );


    std::string text;
    double numberd = 0.;
    int numberi = 0;
    bool mybool = false;


    EXPECT_TIXI_ERROR( text = tixi::TixiGetTextAttribute(-1,"plane/name","non_existing_attribute"),INVALID_HANDLE );
    EXPECT_TIXI_ERROR( text = tixi::TixiGetAttribute<std::string>(documentHandle,"/plane/wings/wing[1]","non_existing_attribute"), ATTRIBUTE_NOT_FOUND);
    EXPECT_TIXI_ERROR( text = tixi::TixiGetAttribute<std::string>(documentHandle,"/plane/wings/wing","position"),ELEMENT_PATH_NOT_UNIQUE );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi::TixiGetAttribute<std::string>( documentHandle,  "/plane/wings", "numberOfWings" ) );
    EXPECT_EQ( text, "2");

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberd=tixi::TixiGetAttribute<double>( documentHandle, "/plane/coordinateOrigin", "scaling" ) );
    EXPECT_EQ( numberd, 1.3456);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberi=tixi::TixiGetAttribute<int>( documentHandle,  "/plane/wings", "numberOfWings" ) );
    EXPECT_EQ( numberi, 2);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( mybool=tixi::TixiGetAttribute<bool>( documentHandle, "/plane/wings/wing[1]", "top" ) );
    ASSERT_EQ(true, mybool);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( mybool=tixi::TixiGetAttribute<bool>( documentHandle, "/plane/wings/wing[2]", "top" ) );
    ASSERT_EQ(false, mybool);
}

TEST(CppWrapper,checkRemoveAttribute){
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument("TestData/in.xml") );
    EXPECT_EQ( tixi::TixiCheckAttribute( documentHandle, "/plane/wings", "numberOfWings"),true );
    ASSERT_NO_THROW( tixi::TixiRemoveAttribute( documentHandle, "/plane/wings", "numberOfWings" ) );
    EXPECT_EQ(tixi::TixiCheckAttribute( documentHandle, "/plane/wings", "numberOfWings"),false );
}

TEST(CppWrapper,getElement)
{
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument("TestData/in.xml") );

    std::string text;
    double numberd;
    int numberi;
    bool myBool = false;

    /*--------------------------------------------------------------------*/
    EXPECT_TIXI_ERROR( text=tixi::TixiGetElement<std::string>( -1            , "/plane/name/pp"       ),INVALID_HANDLE );
    EXPECT_TIXI_ERROR( text=tixi::TixiGetElement<std::string>( documentHandle, "/plane/name/pp"       ),ELEMENT_NOT_FOUND );
    EXPECT_TIXI_ERROR( text=tixi::TixiGetElement<std::string>( documentHandle, "cc/plane/na|<<me/pp"  ),INVALID_XPATH );
    EXPECT_TIXI_ERROR( text=tixi::TixiGetElement<std::string>( documentHandle, "/plane/wings/wing"    ),ELEMENT_PATH_NOT_UNIQUE );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi::TixiGetElement<std::string>( documentHandle, "/plane/empty" ) );
    EXPECT_EQ( text, "");

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi::TixiGetElement<std::string>( documentHandle, "/plane/name" ) );
    EXPECT_EQ( text, "Junkers JU 52");

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberd=tixi::TixiGetElement<double>( documentHandle, "/plane/wings/wing[1]/centerOfGravity/x" ) );
    EXPECT_EQ( numberd, 30.0);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberi=tixi::TixiGetElement<int>( documentHandle, "/plane/numberOfPassengers" ) );
    EXPECT_EQ( numberi, 57);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( myBool=tixi::TixiGetElement<bool>( documentHandle, "/plane/bool/aBool1" ) );
    EXPECT_EQ( myBool, true);
}

TEST(CppWrapper,checkRemoveElement){
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument("TestData/in.xml") );
    EXPECT_EQ( tixi::TixiCheckElement( documentHandle, "/plane/aPoint/point/z"), true );
    ASSERT_NO_THROW( tixi::TixiRemoveElement( documentHandle, "/plane/aPoint/point/z" ) );
    EXPECT_EQ( tixi::TixiCheckElement( documentHandle, "/plane/aPoint/point/z"), false );
}

TEST(CppWrapper,saveAttribute){

    std::string rootElementName = "rootElement";
    ASSERT_NO_THROW( documentHandle = tixi::TixiCreateDocument( rootElementName ) );
    ASSERT_NO_THROW( tixi::TixiCreateElement(documentHandle, "/rootElement/element") );

    ASSERT_NO_THROW( tixi::TixiSaveAttribute( documentHandle, "/rootElement/element","sattribute","stringAttribute") );
    ASSERT_NO_THROW( tixi::TixiSaveAttribute( documentHandle, "/rootElement/element","dattribute",3.14159) );
    ASSERT_NO_THROW( tixi::TixiSaveAttribute( documentHandle, "/rootElement/element","iattribute",4711) );
    ASSERT_NO_THROW( tixi::TixiSaveAttribute( documentHandle, "/rootElement/element","battribute",true) );
}

TEST(CppWrapper,getNamedChildrenCount){
    ASSERT_NO_THROW( documentHandle=tixi::TixiOpenDocument("TestData/in.xml") );

    int count=-1;

    ASSERT_NO_THROW( count = tixi::TixiGetNamedChildrenCount( documentHandle, "/plane/wings/this_child_does_not_exist") );
    ASSERT_TRUE( count  == 0 );

    ASSERT_NO_THROW( count = tixi::TixiGetNamedChildrenCount( documentHandle, "/plane/wings") );
    ASSERT_TRUE( count  == 1 );

    ASSERT_NO_THROW( count = tixi::TixiGetNamedChildrenCount( documentHandle, "/plane/wings/wing") );
    ASSERT_TRUE( count  == 2 );
}
