/*
 * This is an example unit test that doesn't really do anything useful.
 * It is here as a reference for you when creating additional unit tests.
 */

#include "test.h" // Brings in the GTest framework

#include "tixi.h"

TEST(createDocument, noElementName)
{
    char* rootElementName = NULL;
    TixiDocumentHandle handle = -1;

    ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == NO_ELEMENT_NAME );
}

TEST(createDocument, invalidRootNodeName)
{
    char* rootElementName = "1";
    TixiDocumentHandle handle = -1;

    ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == INVALID_XML_NAME );
}

TEST(createDocument, createDocumentSuccess)
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

TEST(createDocument, createDocumentAddHeader)
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

TEST(createDocument, createDocumentAddCPACSHeader)
{
    char* rootElementName = "rootElement";
    char* cpacsName = "cpacsName";
    char* cpacsCreator = "cpacsCreator";
    char* cpacsVersion = "cpacsVersion";
    char* cpacsDescription = "cpacsDescription";
    TixiDocumentHandle handle = -1;
    char *text = NULL;

    ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == SUCCESS );
    ASSERT_TRUE( tixiAddCpacsHeader( handle, cpacsName, cpacsCreator, cpacsVersion, cpacsDescription ) == SUCCESS );

    // Now check the different header values
    ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/name", &text ) == SUCCESS );
    ASSERT_TRUE( !strcmp(text, cpacsName));

    ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/creator", &text ) == SUCCESS );
    ASSERT_TRUE( !strcmp(text, cpacsCreator));

    ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/version", &text ) == SUCCESS );
    ASSERT_TRUE( !strcmp(text, cpacsVersion));

    ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/description", &text ) == SUCCESS );
    ASSERT_TRUE( !strcmp(text, cpacsDescription));
}

