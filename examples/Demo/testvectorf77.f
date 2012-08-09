C
C	  Created: 2010-08-13 Markus.Litz <Markus.Litz@dlr.de>
C     $Id: addElement_f.f 375 2005-06-07 09:53:40Z hpk $
C	  Version: $Revision: 3978 $
C
      program testvectorf77

      implicit none
      include "tixif.h"

      character*100 xml_file_name
      integer i
      integer handle
      integer error
      integer vecSize
      real*8 aray(8)
      character*100 tool_name
      character*15 attributes

      do i=1,100
         xml_file_name(i:i) = " "
      enddo

      xml_file_name = "testvector.xml"

      call tixi_open_document( xml_file_name, handle, error )

      write(*,*) "handle=", handle

      call tixi_get_text_element(handle, '/toolInput/header/tool/name',
     C                            tool_name, error )
      write(*,*) 'Tool name',tool_name


      call tixi_get_vector_size( handle, '/toolInput/header/tool/v',
     C                            vecSize, error )
        write(*,*) 'Number of elements',vecSize
        write(*,*) 'tixi_get_vector_size error: ',error


      call tixi_get_float_vector( handle, '/toolInput/header/tool/v',
     C                             aray, 5, error )
        write(*,*) 'array: ',aray
        write(*,*) 'tixi_get_float_vector error: ',error


      call tixi_add_float_vector(handle, '/toolInput/header/tool', 'y',
     C                            aray, 5, error )
        write(*,*) 'tixi_add_float_vector error:',error

      call tixi_save_document( handle, "blub.xml", error)

      call tixi_close_document( handle, error )

      end
