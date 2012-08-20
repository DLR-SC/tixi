C
C	  Created: 2010-08-13 Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
C     $Id: testf77.f 4392 2011-02-03 08:52:55Z litz_ma $
C	  Version: $Revision: 4392 $
C
      programm tixif77test

      include "tixi77.h"

      character*100 xml_file_name
      integer handle
      integer error
      integer three /3/
      real*8 numbers(3) /0.1, 1000., 0.001/
      character*100 tool_name
      character*15 attributes 

      do i=1,100
         xml_file_name(i:i) = " "
      enddo

      xml_file_name = "demo.xml"
      
      call tixi_open_document( xml_file_name, handle, error )

      write(*,*) "handle=", handle

      call tixi_get_text_element( handle, '/toolInput/header/tool/name',
     $     tool_name, error )

      write(*,*) 'Tool name',tool_name
      
      call tixi_close_document( handle, error )
      
      call tixi_create_document( 'blub', handle, error )

      call tixi_add_text_element( handle, '/blub', 'f90test', 'yup', 
     $     error )

C     The attribues values are packed into a single string which is
c     cutinto equal sized pieces inside
c     tixi_add_double_list_with_attributes.
c     Therefore its size must by nValues * len. 
c     Where len = max(length(attributes(i))) and the ith attribute
c     value must begin at positon (i-1)*len+1. Unused entries in the
c     string should be set to the blank string " ".
c     The example has 3 attributes of length five, each.

      attributes = 'smalllargesmall'

      call tixi_add_double_list_with_attributes( 
     $     handle, '/blub',
     $     'numbers', 'number',
     $     'size', numbers, 
     $     '%g', attributes,
     $     3, error )
c
c     check error code

      if ( error .eq. SUCCESS ) then
         write(*,*) 'Added list to document'
      else
         write(*,*) 'Failed to add list to document'
      endif

      call tixi_save_document( handle, "blub.xml", error)

c
c     try to add an element to an already saved file.
c     This should fail!

      call tixi_add_text_element( handle, '/blub', 'f90test', 'yup', 
     $     error )
      
      if ( error .eq. SUCCESS ) then
         write(*,*) 'This should not happen.'
      else
         write(*,*) 'Main: Failed to add elemnt to document'
      endif

      call tixi_close_document( handle, error ) 

      end

