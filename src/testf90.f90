!C
!C	  Created: 2010-08-13 Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
!C     $Id: testf90.f90 4392 2011-02-03 08:52:55Z litz_ma $
!C	  Version: $Revision: 4392 $
!C
program tixif90test

implicit none

integer i

character (len=100) ::  xml_file_name 

integer handle

integer error	

do i=1,100
   xml_file_name(i:i) = " "
enddo

xml_file_name = 'demo.xml'

call tixi_open_document( xml_file_name, handle, error )

write(*,*) "handle=", handle

call tixi_close_document( handle, error )

call tixi_create_document( 'blub', handle, error )

call tixi_add_text_element( handle, '/blub', 'f90test', 'yup', error )

call tixi_save_document( handle, "blub.xml", error)

call tixi_close_document( handle, error ) 

end program


