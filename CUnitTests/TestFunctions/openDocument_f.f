C
C	  Created: 2010-08-13 Markus.Litz <Markus.Litz@dlr.de>
C     $Id: openDocument_f.f 4392 2011-02-03 08:52:55Z litz_ma $
C	  Version: $Revision: 4392 $
C
      subroutine ftest_open_document_success( error )

      implicit none

      character*15 file_name
      integer error
      integer handle
      common handle

      file_name = 'TestData/in.xml'
      call tixi_open_document( file_name, handle, error )
      
      end

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      subroutine ftest_close_document_success( error )

      implicit none

      integer error
      integer handle
      common handle

      
      call tixi_close_document(  handle, error )
      
      end

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
