/*
 * This is an example unit test that doesn't really do anything useful.
 * It is here as a reference for you when creating additional unit tests.
 */

#include "test.h" // Brings in the GTest framework

#include "tixi.h"

TEST(createDocument, noElementName) // Declares a test named "sanity_check"
{
    char* rootElementName = NULL;
    TixiDocumentHandle handle = -1;

    ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == NO_ELEMENT_NAME );
}

TEST(createDocument, invalidRootNodeName) // Declares a test named "sanity_check"
{
    char* rootElementName = "1";
    TixiDocumentHandle handle = -1;

    ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == INVALID_XML_NAME );
}

TEST(createDocument, createDocumentSuccess) // Declares a test named "sanity_check"
{
    char* rootElementName = "rootElement";
    TixiDocumentHandle handle = -1;

    ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == SUCCESS );
    ASSERT_TRUE( handle != -1 );
    if ( handle != -1 ) {
        ASSERT_TRUE( tixiSaveDocument( handle, "createDocument_success.xml" )== SUCCESS );
        ASSERT_TRUE( tixiCloseDocument( handle )== SUCCESS );
    }
}

TEST(createDocument, createDocumentAddHeader) // Declares a test named "sanity_check"
{
    char* rootElementName = "rootElement";
    TixiDocumentHandle handle = -1;

    ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == SUCCESS );

    if ( handle != -1 ) {
        ASSERT_TRUE( tixiAddHeader( handle, "ToolName", "4.7.11", "Rudi Ratlos" ) == SUCCESS );
        ASSERT_TRUE( tixiSaveDocument( handle, "documentWithHeader.xml" )== SUCCESS );
        ASSERT_TRUE( tixiCloseDocument( handle )== SUCCESS );

#if __unix__ || unix || __APPLE__
        system( "sed  /timestamp/d documentWithHeader.xml >tmp.xml");
        ASSERT_TRUE( system("diff -w tmp.xml TestData/documentWithHeaderReference.xml" ) == 0 );
#endif /* __unix__ */
    }
}

