//==============================================================================
//
//  Project realization   
//
//==============================================================================
//
//  externals: 

    #include "Project.hpp"


//==============================================================================
//
//  publics:

void            
Project::process_file( const std::string& name )
{
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    
    parser->setContentHandler( this );
    parser->setErrorHandler( this );
    parser->parse( name.c_str() );
}


//------------------------------------------------------------------------------

void            
Project::startElement( const XMLCh* const uri, 
                       const XMLCh* const localname, 
                       const XMLCh* const qname, 
                       const Attributes& attrs
                     )
{
     printf("elem\n");
}


//------------------------------------------------------------------------------

void            
Project::Project::characters( const XMLCh* const chars, 
                              const unsigned int length
                            )
{
     
}


//------------------------------------------------------------------------------

void            
Project::ignorableWhitespace( const XMLCh* const chars,
                     const unsigned int length
                   )
{
     
}


//------------------------------------------------------------------------------

void            
Project::resetDocument()
{
     
}



