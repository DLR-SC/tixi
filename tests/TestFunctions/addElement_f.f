C
C	  Created: 2010-08-13 Markus.Litz <Markus.Litz@dlr.de>
C     $Id: addElement_f.f 4392 2011-02-03 08:52:55Z litz_ma $
C	  Version: $Revision: 4392 $
C
      subroutine ftest_add_element_add_tree(  error )

      implicit none

      integer error, e
      integer handle
      character*15 file_name

      call tixi_create_document( 'rootElement', handle, error  )

      call tixi_add_text_element(  handle, '/rootElement', 'flevel1',
     $     "", error )
       call tixi_add_double_element(  handle, '/rootElement/flevel1', 
     $     'flevel2', 2.7d0, '%10.3f', error )

      call tixi_add_double_element(  handle, '/rootElement/flevel1',
     $     'flevel2', 2.7d0, '', error )

      call tixi_save_document( handle, 'ftest.xml', e )
      call tixi_close_document( handle, e )

      end



      subroutine ftest_add_point ( error )

      implicit none

      integer error, e
      integer handle
      
      real *8 x,y,z

      x = 1.
      y = 2.34
      z = 3.14159265

      call tixi_create_document( 'rootElement', handle, error  )

      call tixi_add_point( handle, '/rootElement', x, y, z, 
     $     "%10.4f", error )

      call tixi_save_document( handle, 'ftestaddpoint.xml', e )
      call tixi_close_document( handle, e )

      end

C----------------------------------------------------------------------

      subroutine ftest_get_added_point( x, y, z, error )

      implicit none

      integer error, e
      integer handle
      real*8 x, y, z


      call tixi_open_document( 'ftestaddpoint.xml', handle, error  )
      call tixi_get_point(  handle, '/rootElement', 1, x, y, z, error )
      call tixi_close_document( handle, e )

      end

C----------------------------------------------------------------------

      subroutine ftest_add_matrix( error )

      implicit none
      
      include 'tixif.h'

      integer error, e
      integer handle
      character*20 matrix_name / 'fmatrix' /

      real*8 array(3,3)

      data array/11., 21., 31., 12., 22., 32., 13., 23., 33./

      call tixi_create_document( 'root', handle, error  )

      call tixi_add_float_matrix( handle, '/root', matrix_name, 'frow',
     $     'fcol', 3, 3, COLUMN_WISE, array, "", error )

      call tixi_save_document( handle, 'faddmatrix.xml', e )
      call tixi_close_document( handle, e )

      end
