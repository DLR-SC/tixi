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

static TixiDocumentHandle documentHandle = -1;

TEST(CppWrapper, createDocumentInvalidRootNodeName)
{
    std::string rootElementName = "1";

    try {
        documentHandle =  tixi3::TixiCreateDocument( rootElementName );
        FAIL() << "Expected tixi3::helper::TixiError";
    }
    catch(tixi3::helper::TixiError const & err) {
        ASSERT_TRUE(err.returnCode()==INVALID_XML_NAME);
    }
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

  ASSERT_NO_THROW( documentHandle = tixi3::TixiCreateDocument( rootElementName ) );
  ASSERT_NO_THROW( tixi3::TixiAddCpacsHeader( documentHandle, cpacsName, cpacsCreator, version, cpacsDescription, cpacsVersion ) );

  // Now check the different header values
  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/name" ) );
  EXPECT_EQ(text, cpacsName);

  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/creator") );
  EXPECT_EQ(text, cpacsCreator);

  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/version") );
  EXPECT_EQ(text, version);

  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/cpacsVersion") );
  EXPECT_EQ(text, cpacsVersion);

  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/description") );
  EXPECT_EQ(text, cpacsDescription);
}

TEST(CppWrapper,importFromString)
{
    std::string s = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root/>";
    TixiDocumentHandle handle;
    ASSERT_NO_THROW( handle = tixi3::TixiImportFromString(s) );
}

TEST(CppWrapper, exportAsString)
{
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi3::TixiOpenDocument(xmlFilename) );

    std::string text;
    std::string text2;
    ASSERT_NO_THROW( text=tixi3::TixiExportDocumentAsString(documentHandle) );
    try {
        text2=tixi3::TixiExportDocumentAsString(-1);
    }
    catch(tixi3::helper::TixiError const & err) {
        ASSERT_TRUE(err.returnCode()==INVALID_HANDLE);
    }
}

TEST(CppWrapper,getAttributeNames)
{
    TixiDocumentHandle documentHandle;
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi3::TixiOpenDocument(xmlFilename) );

    std::vector<std::string> attributeNames;
    ASSERT_NO_THROW( attributeNames=tixi3::TixiGetAttributeNames(documentHandle,"/plane/aPoint[1]") );
    EXPECT_EQ( attributeNames.size(),2);
    EXPECT_EQ( attributeNames[0],"system");
    EXPECT_EQ( attributeNames[1],"type");
}

TEST(CppWrapper,getAttribute)
{
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi3::TixiOpenDocument(xmlFilename) );


    std::string text;
    double numberd = 0.;
    int numberi = 0;
    bool mybool = false;

    /*--------------------------------------------------------------------*/
    try{
        text = tixi3::TixiGetTextAttribute(-1,"plane/name","non_existing_attribute");
    }
    catch(tixi3::helper::TixiError const & err) {
        ASSERT_TRUE(err.returnCode()==INVALID_HANDLE);
    }

    /*--------------------------------------------------------------------*/
    try{
        text = tixi3::TixiGetAttribute<std::string>(documentHandle,"/plane/wings/wing[1]","non_existing_attribute");
    }
    catch(tixi3::helper::TixiError const & err) {
        ASSERT_TRUE(err.returnCode()==ATTRIBUTE_NOT_FOUND);
    }

    /*--------------------------------------------------------------------*/
    try{
        text = tixi3::TixiGetAttribute<std::string>(documentHandle,"/plane/wings/wing","position");
    }
    catch(tixi3::helper::TixiError const & err) {
        ASSERT_TRUE(err.returnCode()==ELEMENT_PATH_NOT_UNIQUE);
    }

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi3::TixiGetAttribute<std::string>( documentHandle,  "/plane/wings", "numberOfWings" ) );
    EXPECT_EQ( text, "2");

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberd=tixi3::TixiGetAttribute<double>( documentHandle, "/plane/coordinateOrigin", "scaling" ) );
    EXPECT_EQ( numberd, 1.3456);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberi=tixi3::TixiGetAttribute<int>( documentHandle,  "/plane/wings", "numberOfWings" ) );
    EXPECT_EQ( numberi, 2);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( mybool=tixi3::TixiGetAttribute<bool>( documentHandle, "/plane/wings/wing[1]", "top" ) );
    ASSERT_EQ(true, mybool);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( mybool=tixi3::TixiGetAttribute<bool>( documentHandle, "/plane/wings/wing[2]", "top" ) );
    ASSERT_EQ(false, mybool);
}

TEST(CppWrapper,checkRemoveAttribute){
    ASSERT_NO_THROW( documentHandle=tixi3::TixiOpenDocument("TestData/in.xml") );
    ASSERT_NO_THROW( tixi3::TixiCheckAttribute( documentHandle, "/plane/wings", "numberOfWings") );
    ASSERT_NO_THROW( tixi3::TixiRemoveAttribute( documentHandle, "/plane/wings", "numberOfWings" ) );
    try{
        tixi3::TixiCheckAttribute( documentHandle, "/plane/wings", "numberOfWings");
    }
    catch(tixi3::helper::TixiError const & err){
        ASSERT_TRUE(err.returnCode()!=SUCCESS);
    }
}

TEST(CppWrapper,getElement)
{
    ASSERT_NO_THROW( documentHandle=tixi3::TixiOpenDocument("TestData/in.xml") );

    std::string text;
    double numberd;
    int numberi;
    bool myBool = false;

    /*--------------------------------------------------------------------*/
    try{
        text=tixi3::TixiGetElement<std::string>( -1, "/plane/name/pp"  );
    }
    catch(tixi3::helper::TixiError const & err){
        ASSERT_TRUE(err.returnCode()==INVALID_HANDLE);
    }

    /*--------------------------------------------------------------------*/
    try{
        text=tixi3::TixiGetElement<std::string>( documentHandle, "/plane/name/pp"  );
    }
    catch(tixi3::helper::TixiError const & err){
        ASSERT_TRUE(err.returnCode()==ELEMENT_NOT_FOUND);
    }

    /*--------------------------------------------------------------------*/
    try{
        text=tixi3::TixiGetElement<std::string>( documentHandle, "cc/plane/na|<<me/pp"  );
    }
    catch(tixi3::helper::TixiError const & err){
        ASSERT_TRUE(err.returnCode()==INVALID_XPATH);
    }

    /*--------------------------------------------------------------------*/
    try{
        text=tixi3::TixiGetElement<std::string>( documentHandle, "/plane/wings/wing"  );
    }
    catch(tixi3::helper::TixiError const & err){
        ASSERT_TRUE(err.returnCode()==ELEMENT_PATH_NOT_UNIQUE);
    }

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi3::TixiGetElement<std::string>( documentHandle, "/plane/empty" ) );
    EXPECT_EQ( text, "");

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( text=tixi3::TixiGetElement<std::string>( documentHandle, "/plane/name" ) );
    EXPECT_EQ( text, "Junkers JU 52");

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberd=tixi3::TixiGetElement<double>( documentHandle, "/plane/wings/wing[1]/centerOfGravity/x" ) );
    EXPECT_EQ( numberd, 30.0);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( numberi=tixi3::TixiGetElement<int>( documentHandle, "/plane/numberOfPassengers" ) );
    EXPECT_EQ( numberi, 57);

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( myBool=tixi3::TixiGetElement<bool>( documentHandle, "/plane/bool/aBool1" ) );
    EXPECT_EQ( myBool, true);
}

TEST(CppWrapper,checkRemoveElement){
    ASSERT_NO_THROW( documentHandle=tixi3::TixiOpenDocument("TestData/in.xml") );
    ASSERT_NO_THROW( tixi3::TixiCheckElement( documentHandle, "/plane/aPoint/point/z") );
    ASSERT_NO_THROW( tixi3::TixiRemoveElement( documentHandle, "/plane/aPoint/point/z" ) );
    try{
        tixi3::TixiCheckElement( documentHandle, "/plane/aPoint/point/z");
    }
    catch(tixi3::helper::TixiError const & err){
        ASSERT_TRUE(err.returnCode()!=SUCCESS);
    }
}

TEST(CppWrapper,saveAttribute){

    std::string rootElementName = "rootElement";
    ASSERT_NO_THROW( documentHandle = tixi3::TixiCreateDocument( rootElementName ) );
    ASSERT_NO_THROW( tixi3::TixiSaveElement( documentHandle, "/rootElement/element", "content" ) );

    ASSERT_NO_THROW( tixi3::TixiSaveAttribute( documentHandle, "/rootElement/element","sattribute","stringAttribute") );
    ASSERT_NO_THROW( tixi3::TixiSaveAttribute( documentHandle, "/rootElement/element","dattribute",3.14159) );
    ASSERT_NO_THROW( tixi3::TixiSaveAttribute( documentHandle, "/rootElement/element","iattribute",4711) );
    ASSERT_NO_THROW( tixi3::TixiSaveAttribute( documentHandle, "/rootElement/element","battribute",true) );
}

TEST(CppWrapper,saveElement){
    std::string rootElementName = "rootElement";
    ASSERT_NO_THROW( documentHandle = tixi3::TixiCreateDocument( rootElementName ) );

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( tixi3::TixiSaveElement( documentHandle, "/rootElement","level1") );
    ASSERT_NO_THROW( tixi3::TixiSaveElement( documentHandle, "/rootElement/level1","level2") );

    /*--------------------------------------------------------------------*/
    try{
        tixi3::TixiSaveElement( documentHandle, "/rootElement/newParent","withParent" );
    }
    catch(tixi3::helper::TixiError const & err){
        ASSERT_TRUE(err.returnCode()==ELEMENT_NOT_FOUND);
    }

    /*--------------------------------------------------------------------*/
    ASSERT_NO_THROW( tixi3::TixiSaveElement( documentHandle, "/rootElement/double",3.14159265) );
    ASSERT_NO_THROW( tixi3::TixiSaveElement( documentHandle, "/rootElement/int",4711) );
    ASSERT_NO_THROW( tixi3::TixiSaveElement( documentHandle, "/rootElement/bool_t",true) );
    ASSERT_NO_THROW( tixi3::TixiSaveElement( documentHandle, "/rootElement/bool_f",false) );
    try{
        tixi3::TixiSaveElement( documentHandle, "/rootElement/bool_x",2);
    }
    catch(tixi3::helper::TixiError const & err){
        ASSERT_TRUE(err.returnCode()!=SUCCESS);
    }

    std::vector<std::string> children(5);
    children[0]="child1";
    children[1]="child2";
    children[2]="child3";
    children[3]="child4";
    children[4]="child5";
    ASSERT_NO_THROW( tixi3::TixiSaveElements<std::string>( documentHandle, "/rootElement/children",children) );
}

TEST(CppWrapper,getNamedChildrenCount){
    ASSERT_NO_THROW( documentHandle=tixi3::TixiOpenDocument("TestData/in.xml") );

    int count=-1;

    ASSERT_NO_THROW( count = tixi3::TixiGetNamedChildrenCount( documentHandle, "/plane/wings/this_child_does_not_exist") );
    ASSERT_TRUE( count  == 0 );

    ASSERT_NO_THROW( count = tixi3::TixiGetNamedChildrenCount( documentHandle, "/plane/wings") );
    ASSERT_TRUE( count  == 1 );

    ASSERT_NO_THROW( count = tixi3::TixiGetNamedChildrenCount( documentHandle, "/plane/wings/wing") );
    ASSERT_TRUE( count  == 2 );
}
