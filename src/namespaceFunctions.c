#include "namespaceFunctions.h"


#include "tixiUtils.h"
#include <libxml/tree.h>

#include <stdlib.h>
#include <string.h>

extern void printMsg(MessageType type, const char* message, ...);

void extractPrefixAndName(const char *qualifiedName, char** prefix, char** name)
{
  char* chr = strchr(qualifiedName, ':');

  if (!chr) {
    /* no prefix defined */
    *prefix = NULL;
    *name = substring(qualifiedName, 0, -1);
  }
  else {
    int pos = chr - qualifiedName;
    *prefix = substring(qualifiedName, 0, pos-1);
    *name = substring(qualifiedName, pos+1, -1);
  }
}

xmlNsPtr findNamespace(const xmlDocPtr document, const xmlNodePtr node, const char *namespaceURI)
{
  xmlNsPtr* nsArray = NULL;

  if (!document) {
    printMsg(MESSAGETYPE_ERROR, "Null pointer for xml document!");
    return NULL;
  }

  if (!node) {
    printMsg(MESSAGETYPE_ERROR, "Null pointer for xml node!");
    return NULL;
  }

  nsArray = xmlGetNsList(document, node);

  if (!nsArray) {
    return NULL;
  }
  else {
    int iNS = 0;
    xmlNsPtr ns = NULL;
    for (iNS = 0; nsArray[iNS] != NULL; ++iNS) {
      ns = nsArray[iNS];

      if (strcmp(namespaceURI, (char*) ns->href)==0) {
        /* namespace found */
        return ns;
      }
    }

    return NULL;
  }
}


xmlNsPtr nodeSetNamespace(const xmlDocPtr doc, xmlNodePtr node, const char *nsPrefix, const char *namespaceURI)
{
  xmlNsPtr ns = NULL;

  if (!node || !doc || !namespaceURI) {
    return ns;
  }

  /* check if namespace already exists. */
  if (node->parent) {
    ns = findNamespace(doc, node->parent, namespaceURI);
  }

  /* if the prefix is an empty string, make it null */
  if (nsPrefix && strcmp(nsPrefix, "") == 0) {
      nsPrefix = NULL;
  }

  if (!ns || (nsPrefix && (!ns->prefix || strcmp(nsPrefix, (char*)ns->prefix) != 0))) {
    /* create new namespace defintion, if the prefixes don't match or no namespace was found */
    ns = xmlNewNs(node, (xmlChar*)namespaceURI, (xmlChar*)nsPrefix);
  }

  xmlSetNs(node, ns);

  return ns;
}


xmlNsPtr nodeAddNamespace(const xmlDocPtr doc, xmlNodePtr node, const char *nsPrefix, const char *namespaceURI)
{
    xmlNsPtr ns = NULL;

    if (!nsPrefix || strcmp(nsPrefix, "") == 0) {
      return ns;
    }

    if (!node || !doc || !namespaceURI) {
      return ns;
    }

    /* check if namespace already exists. */
    if (node->parent) {
      ns = findNamespace(doc, node->parent, namespaceURI);
    }

    if (!ns || (nsPrefix && (!ns->prefix || strcmp(nsPrefix, (char*)ns->prefix) != 0))) {
      /* create new namespace defintion, if the prefixes don't match or no namespace was found */
      ns = xmlNewNs(node, (xmlChar*)namespaceURI, (xmlChar*)nsPrefix);
    }

    return ns;
}
