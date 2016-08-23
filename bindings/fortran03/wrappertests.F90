! #############################################################################
! # Copyright (C) 2015 German Aerospace Center (DLR/SC)
! #
! # Created: 2015-09-16 Melven Roehrig-Zoellner <Melven.Roehrig-Zoellner@DLR.de>
! #
! # Licensed under the Apache License, Version 2.0 (the "License");
! # you may not use this file except in compliance with the License.
! # You may obtain a copy of the License at
! #
! #     http://www.apache.org/licenses/LICENSE-2.0
! #
! # Unless required by applicable law or agreed to in writing, software
! # distributed under the License is distributed on an "AS IS" BASIS,
! # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
! # See the License for the specific language governing permissions and
! # limitations under the License.
! #############################################################################

program wrappertest
  use, intrinsic :: iso_c_binding
  use tixi
  implicit none

  call test_version
  call test_open
  call test_open_http
  call test_open_string
  call test_create
  call test_multiple_tixis
  call test_validation_and_export
  call test_elements
  call test_attributes
  call test_vector_array
  call test_api

  call test_tixicleanup

contains

  subroutine print_error_and_exit(line)
    integer, intent(in) :: line
    write(*,*) 'ERROR in line', line
    call exit(1)
  end subroutine
#define VERIFY(x) if( .not. x ) call print_error_and_exit(__LINE__)

  function str_array_eq(str_a, str_b) result(eq)
    character, intent(in) :: str_a(:)
    character(len=*), intent(in) :: str_b
    logical :: eq
    integer :: i

    if( size(str_a) .ne. len(str_b) ) then
      eq = .false.
      return
    end if

    eq = .true.
    do i = 1, len(str_b)
      eq = eq .and. str_a(i) .eq. str_b(i:i)
    end do
  end function


  subroutine test_version
    character, pointer :: version(:) => null()
    write(*,*) 'test_version'
    call tixiGetVersion(version)
    VERIFY( str_array_eq(version, TIXI_VERSION) )
    write(*,*) 'ok'
  end subroutine


  subroutine test_open
    character(len=*), parameter :: invalid_file = '____HOPEFULLY_THIS_FILE_NAME_DOES_NOT_EXIST'
    character(len=*), parameter :: illformed_file = 'TestData/illformed.xml'
    character(len=*), parameter :: working_file = 'TestData/in.xml'
    integer :: t_handle

    write(*,*) 'test_open'

    VERIFY( SUCCESS .ne. tixiOpenDocument(invalid_file,t_handle) )

    VERIFY( SUCCESS .ne. tixiOpenDocument(illformed_file,t_handle) )

    VERIFY( SUCCESS .eq. tixiOpenDocument(working_file,t_handle) )
    VERIFY( t_handle .ne. -1 )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )


    write(*,*) 'ok'
  end subroutine


  subroutine test_open_http
    character(len=*), parameter :: working_url = 'http://www.w3schools.com/XML/note.xml'
    integer :: t_handle
    write(*,*) 'test_open_http'

    VERIFY( SUCCESS .eq. tixiOpenDocumentFromHttp(working_url,t_handle) )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_open_string
    character(len=*), parameter :: cpacs = '<?xml version="1.0"?>&
                                           <cpacs><header><version>1.2.3&
                                           </version></header></cpacs>'
    integer :: t_handle
    write(*,*) 'test_open_string'

    VERIFY( SUCCESS .eq. tixiImportFromString(cpacs,t_handle) )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_create
    integer :: t_handle
    write(*,*) 'test_create'

    VERIFY( SUCCESS .eq. tixiCreateDocument('cpacs',t_handle) )
    VERIFY( t_handle .ne. -1 )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_multiple_tixis
    integer :: t1_handle, t2_handle
    character(len=*), parameter :: working_file = 'TestData/in.xml'
    write(*,*) 'test_multiple_tixis'

    VERIFY( SUCCESS .eq. tixiOpenDocument(working_file,t1_handle) )
    VERIFY( t1_handle .ne. -1 )
    VERIFY( SUCCESS .eq. tixiOpenDocument(working_file,t2_handle) )
    VERIFY( t2_handle .ne. -1 )
    VERIFY( t1_handle .ne. t2_handle )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t1_handle) )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t2_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_validation_and_export
    use iso_c_binding
    character(len=*), parameter :: valid_CPACS = 'TestData/valid_CPACS_dokumentiert.xml'
    character(len=*), parameter :: header = '<?xml version="1.0" encoding="UTF-8"?>'
    character(len=*), parameter :: valid_schema = 'TestData/valid_cpacs_schema.xsd'
    character(len=*), parameter :: invalid_schema = 'invalid'
    character(len=*), parameter :: other_schema = '<?xml version="1.0" encoding="UTF-8"?>&
<?xml-stylesheet href="xs3p.xsl" type="text/xsl"?>&
<xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns="http://www.w3.org/1999/xhtml"&
targetNamespace="http://www.w3.org/1999/xhtml" &
elementFormDefault="qualified" attributeFormDefault="unqualified">&
</xsd:schema>'
    integer :: i
    integer :: t_handle
    character, pointer :: str(:) => null()
    write(*,*) 'test_validation_and_export'

    VERIFY( SUCCESS .eq. tixiOpenDocument(valid_CPACS,t_handle) )
    VERIFY( SUCCESS .eq. tixiExportDocumentAsString(t_handle,str) )
    VERIFY( size(str) .ge. len(header) )
    VERIFY( str_array_eq(str(1:len(header)),header) )
    VERIFY( SUCCESS .eq. tixiSchemaValidateFromFile(t_handle,valid_schema) )
    VERIFY( SUCCESS .eq. tixiSchemaValidateWithDefaultsFromFile(t_handle,valid_schema) )
    VERIFY( OPEN_SCHEMA_FAILED .eq. tixiSchemaValidateFromString(t_handle,invalid_schema) )
    VERIFY( NOT_SCHEMA_COMPLIANT .eq. tixiSchemaValidateFromString(t_handle,other_schema) )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_elements
    character(len=*), parameter :: working_file = 'TestData/in.xml'
    character(len=*), parameter :: plane_wing_x = &
      &                            '/plane/wings[1]/wing[1]/centerOfGravity/x'
    character(len=*), parameter :: wing1 = '/plane/wings/wing[1]'
    character, pointer :: str(:) => null()
    integer :: t_handle
    integer :: n
    real(kind=8) :: x
    write(*,*) 'test_elements'

    VERIFY( SUCCESS .eq. tixiOpenDocument(working_file,t_handle) )
    VERIFY( SUCCESS .eq. tixiGetTextElement(t_handle,'/plane/name',str) )
    VERIFY( str_array_eq(str,'Junkers JU 52'))

    VERIFY( SUCCESS .eq. tixiUpdateTextElement(t_handle,'/plane/name','D150') )

    VERIFY( SUCCESS .eq. tixiGetTextElement(t_handle,'/plane/name',str) )
    VERIFY( str_array_eq(str,'D150'))

    VERIFY( SUCCESS .eq. tixiAddTextElement(t_handle,'/plane','name','B747') )
    VERIFY( SUCCESS .eq. tixiGetTextElement(t_handle,'/plane/name[2]',str) )
    VERIFY( str_array_eq(str,'B747'))

    VERIFY( SUCCESS .eq. tixiGetIntegerElement(t_handle,'/plane/numberOfPassengers',n) )
    VERIFY( n .eq. 57 )

    VERIFY( SUCCESS .eq. tixiAddIntegerElement(t_handle,'/plane','nop',123456,'%d') )
    VERIFY( SUCCESS .eq. tixiGetIntegerElement(t_handle,'/plane/nop',n) )
    VERIFY( n .eq. 123456 )

    VERIFY( SUCCESS .eq. tixiGetDoubleElement(t_handle,plane_wing_x, x) )
    VERIFY( x .eq. 30.0 )

    VERIFY( SUCCESS .eq. tixiAddDoubleElement(t_handle,wing1,'cog',123.456_8,'%g') )
    VERIFY( SUCCESS .eq. tixiGetDoubleElement(t_handle,wing1//'/cog', x) )
    VERIFY( abs(x - 123.456_8) .le. 2*epsilon(123.456_8) )

    VERIFY( SUCCESS .eq. tixiGetBooleanElement(t_handle,'/plane/bool/aBool2',n) )
    VERIFY( n .eq. 0 )

    VERIFY( SUCCESS .eq. tixiGetBooleanElement(t_handle,'/plane/bool/aBool1b',n) )
    VERIFY( n .eq. 1 )

    VERIFY( ELEMENT_NOT_FOUND .eq. tixiCheckElement(t_handle,'/plane/xx') )
    VERIFY( SUCCESS .eq. tixiCreateElement(t_handle,'/plane','xx') )
    VERIFY( SUCCESS .eq. tixiCheckElement(t_handle,'/plane/xx') )
    VERIFY( SUCCESS .eq. tixiUpdateTextElement(t_handle,'/plane/xx','value') )
    VERIFY( SUCCESS .eq. tixiGetTextElement(t_handle,'/plane/xx',str) )
    VERIFY( str_array_eq(str,'value') )
    VERIFY( SUCCESS .eq. tixiRemoveElement(t_handle,'/plane/xx') )
    VERIFY( ELEMENT_NOT_FOUND .eq. tixiCheckElement(t_handle,'/plane/xx') )
    VERIFY( ELEMENT_NOT_FOUND .eq. tixiGetTextElement(t_handle,'/plane/xx',str) )

    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_attributes
    character(len=*), parameter :: working_file = 'TestData/in.xml'
    character(len=*), parameter :: wing1 = '/plane/wings/wing[1]'
    character(len=*), parameter :: origin = '/plane/coordinateOrigin'
    character, pointer :: str(:) => null()
    integer :: t_handle
    integer :: n, err
    real(kind=8) :: x
    write(*,*) 'test_attributes'

    VERIFY( SUCCESS .eq. tixiOpenDocument(working_file,t_handle) )

    VERIFY( SUCCESS .eq. tixiGetTextAttribute(t_handle,wing1,'position',str) )
    VERIFY( str_array_eq(str, 'left') )

    VERIFY( SUCCESS .eq. tixiGetIntegerAttribute(t_handle,'/plane/wings','numberOfWings',n) )
    VERIFY( n .eq. 2 )

    VERIFY( SUCCESS .eq. tixiAddIntegerAttribute(t_handle,'/plane','intattr',324,'%d') )
    VERIFY( SUCCESS .eq. tixiGetIntegerAttribute(t_handle,'/plane','intattr',n) )
    VERIFY( n .eq. 324 )
    VERIFY( SUCCESS .eq. tixiRemoveAttribute(t_handle,'/plane','intattr') )
    VERIFY( ATTRIBUTE_NOT_FOUND .eq. tixiCheckAttribute(t_handle,'/plane','intattr') )

    VERIFY( SUCCESS .eq. tixiGetDoubleAttribute(t_handle,origin,'scaling',x) )
    VERIFY( abs(x - 1.3456_8) .le. 2*epsilon(1.3456_8) )

    VERIFY( ATTRIBUTE_NOT_FOUND .eq. tixiCheckAttribute(t_handle,'/plane','doubleattr') )
    VERIFY( SUCCESS .eq. tixiAddDoubleAttribute(t_handle,'/plane','doubleattr',123.456_8,'%f') )
    VERIFY( SUCCESS .eq. tixiCheckAttribute(t_handle,'/plane','doubleattr') )
    VERIFY( SUCCESS .eq. tixiGetDoubleAttribute(t_handle,'/plane','doubleattr',x) )
    VERIFY( abs(x - 123.456_8) .le. 2*epsilon(123.456_8) )
    VERIFY( SUCCESS .eq. tixiRemoveAttribute(t_handle,'/plane','doubleattr') )

    VERIFY( SUCCESS .eq. tixiAddTextAttribute(t_handle,'/plane','attr','val') )
    VERIFY( SUCCESS .eq. tixiGetTextAttribute(t_handle,'/plane','attr',str) )
    VERIFY( str_array_eq(str,'val') )

    VERIFY( SUCCESS .eq. tixiGetNamedChildrenCount(t_handle,'/plane/wings','wing',n) )
    VERIFY( n .eq. 2 )

    err = tixiAddDoubleListWithAttributes(t_handle,'/plane','list','elem','attr', &
      &   (/1.0_8,2.0_8,3.0_8/),'%f',(/'a','b','c'/),3)
    VERIFY( SUCCESS .eq. err )
    VERIFY( SUCCESS .eq. tixiGetTextAttribute(t_handle,'/plane/list/elem[1]','attr',str) )
    VERIFY( str_array_eq(str,'a') )
    VERIFY( SUCCESS .eq. tixiGetDoubleElement(t_handle,'/plane/list/elem[3]',x) )
    VERIFY( abs(x-3.0_8) .le. 2*epsilon(3.0_8) )

    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_vector_array
    character(len=*), parameter :: vectorxml = 'TestData/vectorcount.xml'
    character(len=*), parameter :: arrayxml = 'TestData/arraytests.xml'
    character(len=*), parameter :: working_file = 'TestData/in.xml'
    character(len=*), parameter :: uidcorrxml = 'TestData/uid_correct.xml'
    character(len=*), parameter :: uiddupxml = 'TestData/uid_duplicated.xml'
    character(len=*), parameter :: a_cfx = '/a/aeroPerformanceMap/cfx'
    character(len=*), parameter :: perfmap = '/root/aeroPerformanceMap'
    character(len=*), parameter :: plane_wing_x = &
      &                            '/plane/wings[1]/wing[1]/centerOfGravity/x'
    integer :: t_handle
    integer :: n, i
    real(kind=8) :: x, y, z
    real(kind=8), pointer :: vec(:) => null()
    integer, allocatable :: dimSizes(:)
    character, pointer :: str(:) => null()
    type(CStringPtr), target, allocatable :: strArr(:)
    type(CStringPtr), pointer :: strArrPtr => null()
    real(kind=8), parameter :: myvec(30) = (/(i, i = 100,129)/)
    write(*,*) 'test_vector_array'


    ! vectorcount.xml
    VERIFY( SUCCESS .eq. tixiOpenDocument(vectorxml,t_handle) )

    VERIFY( SUCCESS .eq. tixiGetVectorSize(t_handle,a_cfx,n) )
    VERIFY( n .eq. 32 )

    VERIFY( SUCCESS .eq. tixiGetFloatVector(t_handle,a_cfx,vec,n) )
    VERIFY( size(vec) .eq. 32 )
    VERIFY( abs(1.0_8 - vec(1)) .le. 2*epsilon(1.0_8) )
    VERIFY( abs(2.0_8 - vec(2)) .le. 2*epsilon(1.0_8) )
    VERIFY( abs(3.0_8 - vec(3)) .le. 2*epsilon(1.0_8) )

    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )


    ! manual creation of vector
    VERIFY( SUCCESS .eq. tixiCreateDocument('doc',t_handle) )
    VERIFY( SUCCESS .eq. tixiAddFloatVector(t_handle,'/doc','myvec',myvec, size(myvec), '%g') )

    VERIFY( SUCCESS .eq. tixiGetVectorSize(t_handle,'/doc/myvec',n) )
    VERIFY( n .eq. size(myvec) )
    VERIFY( SUCCESS .eq. tixiGetFloatVector(t_handle,'/doc/myvec',vec,n) )
    VERIFY( associated(vec) )
    VERIFY( size(vec) .eq. size(myvec) )
    do i = 1, size(myvec), 1
      VERIFY( abs(vec(i)-myvec(i)) .le. 2*epsilon(1.0_8) )
    end do
    vec=>null()

    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )


    ! arraytests.xml
    VERIFY( SUCCESS .eq. tixiOpenDocument(arrayxml,t_handle) )
    VERIFY( SUCCESS .eq. tixiGetArrayDimensions(t_handle,perfmap,n) )
    VERIFY( n .eq. 4 )
    allocate(dimSizes(4))
    VERIFY( SUCCESS .eq. tixiGetArrayDimensionSizes(t_handle,perfmap,dimSizes,n) )
    VERIFY( all(dimSizes .eq. (/1,2,3,8/)) )
    VERIFY( n .eq. 48 )

    allocate(strArr(4))
    VERIFY( SUCCESS .eq. tixiGetArrayDimensionNames(t_handle,perfmap,strArr) )
    strArrPtr => strArr(1)
    VERIFY( str_array_eq(strArrPtr%str,'machNumber') )
    strArrPtr => strArr(2)
    VERIFY( str_array_eq(strArrPtr%str,'reynoldsNumber') )
    strArrPtr => strArr(3)
    VERIFY( str_array_eq(strArrPtr%str,'angleOfYaw') )
    strArrPtr => strArr(4)
    VERIFY( str_array_eq(strArrPtr%str,'angleOfAttack') )
    deallocate(strArr)

    allocate(vec(1))
    VERIFY( SUCCESS .eq. tixiGetArrayDimensionValues(t_handle,perfmap,0,vec) )
    VERIFY( abs(vec(1) - 1.0) .le. 2*epsilon(1.0_8) )
    deallocate(vec);allocate(vec(3))
    VERIFY( SUCCESS .eq. tixiGetArrayDimensionValues(t_handle,perfmap,2,vec) )
    VERIFY( maxval(abs(vec - (/0,5,10/))) .le. 2*epsilon(1.0_8) )
    deallocate(vec)

    VERIFY( SUCCESS .eq. tixiGetArrayParameters(t_handle,perfmap,n) )
    VERIFY( n .eq. 7 )
    allocate(strArr(7))
    VERIFY( SUCCESS .eq. tixiGetArrayParameterNames(t_handle,perfmap,strArr) )
    strArrPtr => strArr(1)
    VERIFY( str_array_eq(strArrPtr%str,'cfx') )
    strArrPtr => strArr(2)
    VERIFY( str_array_eq(strArrPtr%str,'cfy') )
    strArrPtr => strArr(3)
    VERIFY( str_array_eq(strArrPtr%str,'cfz') )
    strArrPtr => strArr(4)
    VERIFY( str_array_eq(strArrPtr%str,'cmx') )
    strArrPtr => strArr(5)
    VERIFY( str_array_eq(strArrPtr%str,'cmy') )
    strArrPtr => strArr(6)
    VERIFY( str_array_eq(strArrPtr%str,'cmz') )
    strArrPtr => strArr(7)
    VERIFY( str_array_eq(strArrPtr%str,'def') )
    deallocate(strArr)

    VERIFY( SUCCESS .eq. tixiGetArray(t_handle,perfmap,'cmx',32,vec) ) ! 32 in xml
    x = tixiGetArrayValue(vec,dimSizes,(/0,0,0,7/),4)
    VERIFY( abs(x - 8.0_8) .le. 2*epsilon(1.0_8) )

    ! TODO: check this
    !VERIFY( SUCCESS .eq. tixiGetArrayElementNames(t_handle,perfmap,'vector',str) )
    ! this only returns 'machNumber'
    !VERIFY( SUCCESS .eq. tixiGetArrayElementNames(t_handle,perfmap,'array',str) )
    ! ...

    VERIFY( SUCCESS .eq. tixiGetArrayElementCount(t_handle,perfmap,'array',n) )
    VERIFY( n .eq. 7 )

    VERIFY( SUCCESS .eq. tixiCreateElement(t_handle,'/root','p1') )
    VERIFY( SUCCESS .eq. tixiAddPoint(t_handle,'/root',1.0_8,2.0_8,3.0_8,'%g') )
    VERIFY( SUCCESS .eq. tixiGetPoint(t_handle,'/root',x,y,z) )
    VERIFY( maxval(abs( (/x,y,z/) - (/1.0_8,2.0_8,3.0_8/) )) .le. 2*epsilon(1.0_8) )

    VERIFY( SUCCESS .eq. tixiXPathEvaluateNodeNumber(t_handle,perfmap,n) )
    VERIFY( n .eq. 1 )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    VERIFY( SUCCESS .eq. tixiOpenDocument(working_file,t_handle) )
    VERIFY( SUCCESS .eq. tixiXPathExpressionGetTextByIndex(t_handle,plane_wing_x,1,str) )
    VERIFY( str_array_eq(str,'30.0') )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    VERIFY( SUCCESS .eq. tixiOpenDocument(uidcorrxml,t_handle) )
    VERIFY( SUCCESS .eq. tixiUIDCheckDuplicates(t_handle) )
    VERIFY( SUCCESS .eq. tixiUIDCheckExists(t_handle,'schlumpf') )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    VERIFY( SUCCESS .eq. tixiOpenDocument(uiddupxml,t_handle) )
    VERIFY( UID_NOT_UNIQUE .eq. tixiUIDCheckDuplicates(t_handle) )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_api
    integer :: th
    character(len=*), parameter :: r = '/root'
    character(len=*), parameter :: cp_n = 'name'
    character(len=*), parameter :: cp_c = 'create'
    character(len=*), parameter :: cp_v = 'version'
    character(len=*), parameter :: cp_d = 'description'
    character(len=*), parameter :: cp_cv = 'cpacsVersion'
    write(*,*) 'test_api'

    VERIFY( SUCCESS .eq. tixiCreateDocument('root',th) )
    VERIFY( SUCCESS .eq. tixiAddDoubleElement(th,r,'myDouble',3.2_8,'%g') )
    VERIFY( SUCCESS .eq. tixiAddIntegerElement(th,r,'myInteger',6,'%d') )

    ! Here starts the rest
    VERIFY( SUCCESS .eq. tixiAddBooleanElement(th,r,'myBoolean',0) )
    VERIFY( SUCCESS .eq. tixiUpdateDoubleElement(th,r//'/myDouble',3.14159262_8,'%g') )
    VERIFY( SUCCESS .eq. tixiUpdateIntegerElement(th,r//'/myInteger',7,'%d') )
    VERIFY( SUCCESS .eq. tixiUpdateBooleanElement(th,r//'/myBoolean',1) )
    VERIFY( SUCCESS .eq. tixiAddTextElementAtIndex(th,r,'myNewTextElement','myText',2) )
    VERIFY( SUCCESS .eq. tixiCreateElementAtIndex(th,r,'array',0) )
    VERIFY( SUCCESS .eq. tixiAddFloatVector(th,r,'myFloatVector',(/0.0_8,1.1_8,2.2_8/),3, '%g') )
    VERIFY( SUCCESS .eq. tixiAddExternalLink(th,r,'/externalLink','.xml') )
    VERIFY( SUCCESS .eq. tixiAddHeader(th,'tool','version','author') )
    VERIFY( SUCCESS .eq. tixiAddCpacsHeader(th,cp_n,cp_c,cp_v,cp_d,cp_cv) )
    VERIFY( SUCCESS .eq. tixiUsePrettyPrint(th,1) )
    VERIFY( SUCCESS .eq. tixiUIDCheckLinks(th) )
    VERIFY( SUCCESS .eq. tixiUIDSetToXPath(th,r//'/myBoolean','booleanID') )
    !self.assertEqual(t.uIDGetXPath('booleanID'),r//'/myBoolean') )
    !#t.dTDValidate() )
    !#t.xSLTransformationToFile()
    VERIFY( SUCCESS .eq. tixiSaveDocument(th,'TestData/test_save.xml') )
    VERIFY( SUCCESS .eq. tixiCloseDocument(th) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_tixicleanup
    write(*,*) 'test_tixicleanup'

    VERIFY( SUCCESS .eq. tixiCleanup() )

    write(*,*) 'ok'
  end subroutine test_tixicleanup

end program
