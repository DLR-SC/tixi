C
C	  Created: 2010-08-13 Markus.Litz <Markus.Litz@dlr.de>
C     $Id: getElement_f.f 4392 2011-02-03 08:52:55Z litz_ma $
C	  Version: $Revision: 4392 $
C
      subroutine ftest_get_element_success( name, error )

      implicit none

      integer error, e
      integer handle
      character*15 file_name
      character*100 name

      file_name = 'TestData/in.xml'

      call tixi_open_document( file_name, handle, error  )
      call tixi_get_text_element(  handle, '/plane/name', name, error )
      call tixi_close_document( handle, e )

      end
     
      subroutine ftest_get_point( x, y, z, error )

      implicit none

      integer error, e
      integer handle
      real*8 x, y, z
      character*15 file_name

      file_name = 'TestData/in.xml'

      call tixi_open_document( file_name, handle, error  )
      call tixi_get_point(  handle, '/plane/aPoint', 1, x, y, z, error )
      call tixi_close_document( handle, e )

      end

      subroutine ftest_get_not_a_point( error )

      implicit none

      integer error, e
      integer handle
      real*8 x, y, z
      character*15 file_name

      file_name = 'TestData/in.xml'

      call tixi_open_document( file_name, handle, error  )
      call tixi_get_point(  handle, '/plane/notAPoint', 1, x, y, z, 
     $     error )
      call tixi_close_document( handle, e )

      end

C----------------------------------------------------------------------
      
      subroutine ftest_get_float_matrix( array, error )
      
      implicit none

      include 'tixif.h'

      integer error, e
      integer handle
      real*8 array(3,3)


      call tixi_open_document( 'faddmatrix.xml', handle, error  )

      call tixi_get_float_matrix(  handle, '/root/fmatrix', 'frow',
     $     'fcol', 3, 3, ROW_WISE, array, error )

      call tixi_close_document( handle, e )

      end

C----------------------------------------------------------------------

  
