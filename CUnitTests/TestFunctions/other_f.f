C
C	  Created: 2010-08-13 Markus.Litz <Markus.Litz@dlr.de>
C     $Id: other_f.f 4392 2011-02-03 08:52:55Z litz_ma $
C	  Version: $Revision: 4392 $
C
      subroutine ftest_get_matrix_size( n_rows, n_cols, error )
 
     
      implicit none

      integer error, e
      integer handle
      integer n_rows, n_cols


      call tixi_open_document( 'faddmatrix.xml', handle, error  )

      call tixi_get_matrix_size(  handle, '/root/fmatrix', 'frow',
     $     'fcol', n_rows, n_cols, error )

      call tixi_close_document( handle, e )

      end

C----------------------------------------------------------------------

  
      subroutine ftest_check_document_success( error )
 
     
      implicit none

      integer error, e
      integer handle

      call tixi_open_document( 'faddmatrix.xml', handle, error  )

      call tixi_check_document_handle(  handle, error )

      call tixi_close_document( handle, e )

      end

C----------------------------------------------------------------------
  
      subroutine ftest_check_document_failed( error )
 
     
      implicit none

      integer error
    
      call tixi_check_document_handle( -1 , error )

      end

C----------------------------------------------------------------------
  
      subroutine ftest_check_element_success( error )
 
     
      implicit none

      integer error, e
      integer handle

      call tixi_open_document( 'TestData/in.xml', handle, e  )

      call tixi_check_element(  handle, '/plane', error )

      call tixi_close_document( handle, e )

      end

C----------------------------------------------------------------------
  
      subroutine ftest_check_element_failed( error )
 
     
      implicit none

      integer error, e
      integer handle
    
      call tixi_open_document( 'TestData/in.xml', handle, e  )

      call tixi_check_element(  handle, '/planeplaneplane', error )

      call tixi_close_document( handle, e )

      end
