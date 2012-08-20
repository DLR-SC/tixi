C
C	  Created: 2010-08-13 Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
C     $Id: tixi_fortran_examples.f 4392 2011-02-03 08:52:55Z litz_ma $
C	  Version: $Revision: 4392 $
C

      program howto

      implicit none

      call read_xml( )

      call write_xml()

      end

C----------------------------------------------------------------------

      subroutine read_xml()

      implicit none

      include 'tixif.h'

      character*11 filename /'howtoin.xml'/
      integer error
      integer handle
      character*100 plane_name 
      real*8 number
      character*100 element_name
      integer i_wing
      character*20 position
      integer n_rows , n_cols
      parameter ( n_rows = 2, n_cols = 3 )
      real*8 matrix( n_rows, n_cols )
      integer n_rows_read , n_cols_read
      integer i, j

      call tixi_open_document( filename, handle, error )
      
      call check_error( error, 'tixi_open_document' )

      if ( error.ne.0 ) then
         stop "Fatal error when opening input document"
      endif


C---- read simple text element

      call tixi_get_text_element( handle, '/plane/name', plane_name,
     $     error )
      call check_error( error, 'tixi_get_text_element' )
  
      write (*,*) 'Plane name is: ',  plane_name
      
C---- read simple floating point number element

      call tixi_get_double_element( handle, '/plane/coordinateOrigin/x',
     $     number, error )
      call check_error( error, 'tixi_get_double_element' )
      
      write(*,*) 'Coordinate origin x-coordinate: ', number

C---- element /plane/wings has 3 wing elements.
C     get the y element of each centerOfGravity sub-element.

      do i_wing = 1,3
         
         write( element_name, '(A18,I1,A20)' ) 
     $        '/plane/wings/wing[',
     $        i_wing, 
     $        ']/centerOfGravity/y'
         
         call tixi_get_double_element( handle, element_name, number,
     $        error )
         
         call check_error( error, 'tixi_get_double_element' )
         
         write(*,*) 'Center of gravity y coordinate wing #', i_wing,
     $        ' : ', number 
         
      enddo

C---- read an attribute

C---- first try to read a non-existing attribute

      call tixi_get_text_attribute( handle,  '/plane/wings/wing[3]',
     $     'pstn', position, error )

      call check_error( error, 'tixi_get_text_attribute' )
       
C---- now read an existing attribute

      call tixi_get_text_attribute( handle, '/plane/wings/wing[3]',
     $     'position', position, error )

      call check_error( error, 'tixi_get_text_attribute' )
     
      write (*,*) 'Position of wing #3 is "', position, '"'

C---- read a 2x3 matrix 

C---- first get the matrix size

      call tixi_get_matrix_size( handle, '/plane/two_by_three_matrix', 
     $     'blub', 'bla', n_rows_read, n_cols_read, error )
 
      call check_error( error, 'tixi_get_matrix_size' )

      write (*,*)
      write (*,*) 'Size of matrix "two_by_three_matrix" is',
     $     n_rows_read, 'x', n_cols_read


      if (n_rows_read.ne.n_rows) then
         write(*,*) 'number of rows read(', n_rows_read,
     $        '( differs from array first array dimension.'
      endif

      if (n_cols_read.ne.n_cols) then
         write(*,*) 'number of columns read(', n_cols_read,
     $        '( differs from second array dimension.'
      endif

C---- now read matrix and store it column wise

      call tixi_get_float_matrix( handle, '/plane/two_by_three_matrix', 
     $     'blub', 'bla', n_rows_read, n_cols_read, COLUMN_WISE, matrix,
     $     error )
      call check_error( error, 'tixi_get_float_matrix' )

      write (*,*) 'Matrix entries'
      write (*,*)
      write(*,'(A2,A2)') 'I','J'
      do i = 1, n_rows
         do j = 1, n_cols
            
            write (*,*) i,j, matrix(i,j)

         enddo
      enddo

C---- Finally, the document is closed

      call tixi_close_document( handle, error )
      call check_error( error, 'tixi_close_document' )
      end

C----------------------------------------------------------------------
      
      subroutine write_xml()
      
      implicit none
      
      include 'tixif.h'

      character*12 filename /'howtoout.xml'/
      integer n_wings /6/
      integer handle
      integer error
      integer i, j
      character*100 element_name
      character*10 position
      real*8 array(12)

C---- Create a new document for writing with root element named plane.
c     A file name is attributed to this document on saving.

      call tixi_create_document( "plane", handle, error )
       
      call check_error( error, 'tixi_create_document' )

      if ( error.ne.0 ) then
         stop "Fatal error when creating output document"
      endif

      write (*,*)
      write (*,*) 'Created new document.'
      write (*,*)

C---- First a header containing the tool name and version and the
c     author of the file is added. A timestamp is added automatically,
c     as well. 
      
      call tixi_add_header( handle, 'NoTool', '47.11', 'Me', error )
      call check_error( error, 'tixi_add_header' )

C---- Now we insert a empty elements into the root element to create a
c     hierarchy of elements.

      call tixi_add_text_element(
     $     handle,
     $     '/plane',
     $     'wings',
     $     '',
     $     error )

      call check_error( error, 'tixi_add_text_element' )

      write(*,*)
      write(*,*) 'Inserted empty wings element.'
      write(*,*)
     

C---- Insert empty wing elements into the wings element.

      do i = 1, n_wings

         call tixi_add_text_element( 
     $        handle,
     $        '/plane/wings',
     $        'wing',
     $        '',
     $        error )

      enddo  

      write(*,*)
      write(*,*) 'Inserted empty wings element.'
      write(*,*)
      
C---- Into each wing element insert a position attribute and a
c     wingspan element.

      do i = 1, n_wings
         write( element_name, '(A18,I1,A20)' ) 
     $        '/plane/wings/wing[',
     $        i, 
     $        ']'
         
         if (mod(i,2).eq.0) then
            position = 'left'
         else
            position = 'right'
         endif
         
C----  Add an attribute to the wing element
     
         call tixi_add_text_attribute( handle, element_name, 'position',
     $        position, error )
         
C---- Add an wingspan element

         call tixi_add_double_element( handle, element_name, 'wingspan',
     $        i+0.1d0, '%5.1f', error )
         
      enddo

      write(*,*)
      write(*,*)
     $     'Added position attribute and wingspan element to ', 
     $     'qeach wing element.'
      write(*,*)
     
C----  write a 4x3 matrix

      do i = 1,4
         do j= 1,3
            array((i-1)*3+j) = i * 10. + j
         enddo
      enddo

C---- Write array to matrix in row order. 

      call tixi_add_float_matrix( handle, '/plane', 'four_by_three_row',
     $     'rows', 'columns', 4, 3, ROW_WISE, array, '%5.1f', error  )
      
      call check_error( error, 'tixiAddFloatMatrix' )

C---- Now write the array to the matrix in column order.

      call tixi_add_float_matrix( handle, '/plane', 'four_by_three_col',
     $     'rows', 'columns', 4, 3, COLUMN_WISE, array, '%5.1f', error)
      
      
      call check_error( error, 'tixiAddFloatMatrix' )

      write(*,*)
      write(*,*) 'Added 4x3 matrices'
      write(*,*)
      
C---- Create an empty 3x3 matrix and fill the entries with a composite
c     element.

      
      call tixi_create_matrix( handle, '/plane', 'composite', 'r', 'c',
     $     3, 3, error )

      call check_error( error, 'tixi_create_matrix' )

      do i = 1,3
         do j= 1,3
            
            write( element_name, '(A19,I1,A4,I1,A1)' ) 
     $           '/plane/composite/r[',
     $           i, 
     $           ']/c[',
     $           j,
     $           ']'

C---- Add an empty wing element to the matrix element (i,j)

            call tixi_add_text_element( handle, element_name, 'wing',
     $           '', error )
            call  check_error( error, 'tixi_add_text_element' )

            write( element_name, '(A19,I1,A4,I1,A6)' ) 
     $           '/plane/composite/r[',
     $           i, 
     $           ']/c[',
     $           j,
     $           ']/wing'

C---- Insert a position and a span element into the wing element  

            if (mod(i*j,2).eq.0) then
               position = 'left'
            else
               position = 'right'
            endif

            call tixi_add_text_element( handle, element_name,
     $           'position',position, error )

            call  check_error( error, 'tixi_add_text_element' )

            call tixi_add_double_element( handle, element_name, 'span',
     $           (i+j) * 10.d0, '', error )

            call check_error( error, 'tixi_add_double_element' )

         enddo
      enddo


      write(*,*)
      write(*,*) 'Added matrix with composite entries.'
      write(*,*)


C---- After all elements have been added, save the document to file

      call tixi_save_document( handle, filename, error )
      call check_error( error, 'tixi_save_document' )

C---- Finally, the document is closed

      call tixi_close_document( handle, error )
      call check_error( error, 'tixi_close_document' )


      end
C----------------------------------------------------------------------

      subroutine check_error( error, msg )

      implicit none

      character*(*) msg
      integer error

      if (error.ne.0) then
         write (*,*) 'Error ', error, ': ', msg
      endif
      
      end
