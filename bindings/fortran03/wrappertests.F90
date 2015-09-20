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
  call  test_vector_array
  call test_api

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
    character, pointer :: version(:)
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
targetNamespace="http://www.w3.org/1999/xhtml" elementFormDefault="qualified" attributeFormDefault="unqualified">&
</xsd:schema>'
    integer :: i
    integer :: t_handle
    character, pointer :: str(:)
    write(*,*) 'test_validation_and_export'

    VERIFY( SUCCESS .eq. tixiOpenDocument(valid_CPACS,t_handle) )
    VERIFY( SUCCESS .eq. tixiExportDocumentAsString(t_handle,str) )
    VERIFY( size(str) .ge. len(header) )
    VERIFY( str_array_eq(str(1:len(header)),header) )
    VERIFY( SUCCESS .eq. tixiSchemaValidateFromFile(t_handle,valid_schema) )
    VERIFY( OPEN_SCHEMA_FAILED .eq. tixiSchemaValidateFromString(t_handle,invalid_schema) )
    VERIFY( NOT_SCHEMA_COMPLIANT .eq. tixiSchemaValidateFromString(t_handle,other_schema) )
    VERIFY( SUCCESS .eq. tixiCloseDocument(t_handle) )

    write(*,*) 'ok'
  end subroutine


  subroutine test_elements
    character(len=*), parameter :: working_file = 'TestData/in.xml'
    character(len=*), parameter :: plane_name = '/plane/name'
    character(len=*), parameter :: plane_name2 = '/plane/name[2]'
    character(len=*), parameter :: plane = '/plane'
    character(len=*), parameter :: name = 'name'
    character(len=*), parameter :: plane_passengers = '/plane/numberOfPassengers'
    character, pointer :: str(:)
    integer :: t_handle
    integer :: n
    write(*,*) 'test_elements'

    VERIFY( SUCCESS .eq. tixiOpenDocument(working_file,t_handle) )
    VERIFY( SUCCESS .eq. tixiGetTextElement(t_handle,plane_name,str) )
    VERIFY( str_array_eq(str,'Junkers JU 52'))

    VERIFY( SUCCESS .eq. tixiUpdateTextElement(t_handle,plane_name,'D150') )

    VERIFY( SUCCESS .eq. tixiGetTextElement(t_handle,plane_name,str) )
    VERIFY( str_array_eq(str,'D150'))

    VERIFY( SUCCESS .eq. tixiAddTextElement(t_handle,plane,name,'B747') )
    VERIFY( SUCCESS .eq. tixiGetTextElement(t_handle,plane_name2,str) )
    VERIFY( str_array_eq(str,'B747'))

    VERIFY( SUCCESS .eq. tixiGetIntegerElement(t_handle,plane_passengers,n) )
    VERIFY( n .eq. 57 )

    !t.addIntegerElement("/plane", "nop", 123456, None)
    !self.assertEqual(t.getIntegerElement("/plane/nop"), 123456)
    !self.assertEqual(t.getDoubleElement("/plane/wings[1]/wing[1]/centerOfGravity/x"), 30.0)
    !t.addDoubleElement("/plane/wings/wing[1]", "cog", 123.456, None)
    !self.assertEqual(t.getDoubleElement("/plane/wings/wing[1]/cog"), 123.456)
    !self.assertFalse(t.getBooleanElement("/plane/bool/aBool2"))
    !self.assertTrue(t.getBooleanElement("/plane/bool/aBool1b"))
    !self.assertFalse(t.checkElement("/plane/xx")) #ELEMENT_NOT_FOUND
    !t.createElement("/plane", "xx")
    !t.checkElement("/plane/xx")
    !self.assertEquals(t.getTextElement("/plane/xx"), "")
    !t.updateTextElement("/plane/xx", "value")
    !self.assertEquals(t.getTextElement("/plane/xx"), "value")
    !t.removeElement("/plane/xx")
    !self.assertEqual(t.checkElement("/plane/xx"),False)
    !self.assertRaises(TixiException, t.getTextElement, "/plane/xx") #ELEMENT_NOT_FOUND
    write(*,*) 'ok'
  end subroutine


  subroutine test_attributes
    write(*,*) 'test_attributes'
    !t = Tixi()
    !t.open(os.path.join("TestData", "in.xml"))
    !self.assertEquals(t.getTextAttribute("/plane/wings/wing[1]", "position"), "left")
    !self.assertEquals(t.getIntegerAttribute("/plane/wings", "numberOfWings"), 2)
    !t.addIntegerAttribute("/plane", "intattr", 324, None)
    !self.assertEquals(t.getIntegerAttribute("/plane", "intattr"), 324)
    !t.removeAttribute("/plane", "intattr")
    !self.assertEquals(t.getDoubleAttribute("/plane/coordinateOrigin", "scaling"), 1.3456)
    !self.assertFalse(t.checkAttribute("/plane", "doubleattr")) # ATTRIBUTE_NOT_FOUND \n
    !t.addDoubleAttribute("/plane", "doubleattr", 123.456, None)
    !t.checkAttribute("/plane", "doubleattr")
    !self.assertEquals(t.getDoubleAttribute("/plane", "doubleattr"), 123.456)
    !t.removeAttribute("/plane", "doubleattr")
    !t.addTextAttribute("/plane", "attr", "val")
    !self.assertEquals(t.getTextAttribute("/plane", "attr"), "val")
    !self.assertEquals(t.getNamedChildrenCount("/plane/wings", "wing"), 2)
    !t.addDoubleListWithAttributes("/plane", "list", "elem", "attr", [1., 2., 3.], None, ["a", "b", "c"], 3)
    !self.assertEquals(t.getTextAttribute("/plane/list/elem[1]", "attr"), "a")
    !self.assertEquals(t.getDoubleElement("/plane/list/elem[3]"), 3.0)
    !t.close()
    write(*,*) 'ok'
  end subroutine


  subroutine test_vector_array
    write(*,*) 'test_vector_array'
    !t = Tixi()
    !t.open(os.path.join("TestData", "vectorcount.xml"))
    !self.assertEquals(t.getVectorSize("/a/aeroPerformanceMap/cfx"), 32)
    !#v = t.getFloatVector("/a/aeroPerformanceMap/cfx", 32)
    !#print v[0], v[1], v[-1]
    !#1 2 118
    !t.close()

    !# manual creation of vector
    !t.create('doc')
    !t.addFloatVector('/doc', 'myvec', range(100,130), 30, "%g")

    !size = t.getVectorSize('/doc/myvec')  
    !self.assertEquals(size, 30)
    !v = t.getFloatVector('/doc/myvec', 30)
    !self.assertEquals(v, tuple(range(100,130)))

    !t.close()

    !t.open(os.path.join("TestData", "arraytests.xml"))
    !self.assertEquals(t.getArrayDimensions("/root/aeroPerformanceMap"), 4)
    !self.assertEquals(t.getArrayDimensionSizes("/root/aeroPerformanceMap", 4), ((1, 2, 3, 8), 48))
    !self.assertEquals(t.getArrayDimensionNames("/root/aeroPerformanceMap", 4), ("machNumber", "reynoldsNumber", "angleOfYaw", "angleOfAttack"))
    !# print t.getArrayDimensionValues("/root/aeroPerformanceMap", 0) 1.0
    !# print t.getArrayDimensionValues("/root/aeroPerformanceMap", 2)            0 5 10
    !self.assertEquals(t.getArrayParameters("/root/aeroPerformanceMap"), 7)
    !# print t.getArrayParameterNames("/root/aeroPerformanceMap")            ("cfx", "cfy", "cfz", "cmx", "cmy", "cmz", "def")
    !# print t.getArray("/root/aeroPerformanceMap", "cmx")            None
    !# print t.getArrayValue(arr, "/root/aeroPerformanceMap", "angleOfAttack")
    !#8
    !#print t.getArrayElementNames("/root/aeroPerformanceMap", "vector")
    !#print t.getArrayElementNames("/root/aeroPerformanceMap", "array")
    !t.getArrayElementCount("/root/aeroPerformanceMap","array")
    !t.createElement("/root", "p1")
    !t.addPoint("/root/p1", 1, 2, 3, None)
    !self.assertEquals(t.getPoint("/root/p1"), (1.0, 2.0, 3.0))
    !self.assertEquals(t.xPathEvaluateNodeNumber("/root/aeroPerformanceMap"), 1)
    !t.close()
    !t.open(os.path.join("TestData", "in.xml"))
    !self.assertEquals(t.xPathExpressionGetTextByIndex("/plane/wings/wing/centerOfGravity/x", 2), "30.0")
    !t.close()
    !t.open(os.path.join("TestData", "uid_correct.xml"))
    !t.uIDCheckDuplicates()
    !t.uIDCheckExists("schlumpf")
    !t.close()
    !t.open(os.path.join("TestData", "uid_duplicated.xml"))
    !self.assertRaises(TixiException, t.uIDCheckDuplicates) #UID_NOT_UNIQUE
    write(*,*) 'ok'
  end subroutine


  subroutine test_api
    write(*,*) 'test_api'
    !t = Tixi()
    !self.assertEquals(t.version, t.getVersion())
    !t.create("root")
    !t.addDoubleElement("/root","myDouble",3.2,0)
    !t.addIntegerElement("/root","myInteger",6,None)

    !# Here starts the rest
    !t.addBooleanElement("/root","myBoolean",0)
    !t.updateDoubleElement("/root/myDouble",3.14159262,0)
    !t.updateIntegerElement("/root/myInteger",7,None)
    !t.updateBooleanElement("/root/myBoolean",1)
    !t.addTextElementAtIndex("/root","myNewTextElement","myText",2)
    !t.createElementAtIndex("/root","array",0)
    !t.addFloatVector("/root","myFloatVector",(0.0,1.1,2.2),3, "%g")
    !t.addExternalLink("/root","/externalLink",".xml")
    !t.addHeader("tool","version","author")
    !t.addCpacsHeader("name","creator","version","description","cpacsVersion")
    !t.usePrettyPrint(1)
    !t.uIDCheckLinks()
    !t.uIDSetToXPath("/root/myBoolean","booleanID")
    !self.assertEqual(t.uIDGetXPath("booleanID"),"/root/myBoolean")
    !#t.dTDValidate()
    !#t.xSLTransformationToFile()
    !t.save(os.path.join("TestData", "test_save.xml"))
    !t.close()
    !t.cleanup()
    write(*,*) 'ok'
  end subroutine

end program
