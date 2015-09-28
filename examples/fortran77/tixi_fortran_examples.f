C
C     Created: 2010-08-13 Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
C

      program howto

      implicit none

      integer error

      call read_xml( )

      call write_xml()

      call tixi_cleanup(error)

      end

C----------------------------------------------------------------------

      subroutine read_xml()

      implicit none

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


C---- Finally, the document is closed

      call tixi_close_document( handle, error )
      call check_error( error, 'tixi_close_document' )
      end

C----------------------------------------------------------------------
      
      subroutine write_xml()
      
      implicit none

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
