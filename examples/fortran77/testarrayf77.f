
      program testarrayf77

      implicit none


      character*100 xml_file_name
      integer handle
      integer error
      integer arrayDims
      integer arraySize
      integer*4 sizes(4)
      real*8 aray(8)
      integer i

      xml_file_name(:) = " "

      xml_file_name = "testarray.xml"

      call tixi_open_document( xml_file_name, handle, error )
      write(*,*) "tixi_open_document handle=", handle
      write(*,*) 'tixi_open_document error =', error

      call tixi_get_array_dimensions( handle,
     C                                '/root/aeroPerformanceMap',
     C                                arrayDims, error )
      write(*,*) 'Number of dimensions           =', arrayDims
      write(*,*) 'tixi_get_array_dimensions error=', error

      call tixi_get_array_dimension_sizes( handle,
     C                                      '/root/aeroPerformanceMap',
     C                                     sizes, arraySize, error )
      write(*,*) 'Sizes=', sizes(1), sizes(2), sizes(3), sizes(4)
      write(*,*) 'tixi_get_array_dimension_sizes error=', error

      call tixi_close_document( handle, error )
      write(*,*) 'tixi_close_document error=', error

      end
