# -*-# coding: utf-8 -*-
"""
Created on Wed Sep 16 10:58:02 2015

@author: Melven Roehrig-Zoellner <Melven.Roehrig-Zoellner@DLR.de>
"""

from __future__ import print_function
import sys, os 
from datetime import date

tixipath = os.path.dirname(os.path.realpath(__file__)) + '/../..'
sys.path.append(tixipath + '/bindings')
import bindings_generator.fortran03_generator as FG
import bindings_generator.cheader_parser      as CP

apache = \
'''#############################################################################
# Copyright (C) 2015 German Aerospace Center (DLR/SC)
#
# Created: 2015-09-16 Melven Roehrig-Zoellner <Melven.Roehrig-Zoellner@DLR.de>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#############################################################################

'''
apache += \
'# This file is automatically created from tixi.h on %s.\n'\
    % date.today() + \
'# If you experience any bugs please contact the authors\n\n'

userdeclarations = \
'''
abstract interface
    subroutine TixiPrintMsgFnc(messageType, msg) bind(C)
        use, intrinsic :: iso_c_binding
        integer(kind=C_INT), value :: messageType
        character(kind=C_CHAR), intent(in) :: msg(*)
    end subroutine
end interface
'''

userfunctions = \
'''
subroutine tixiGetVersion(str)
  use, intrinsic :: iso_c_binding
  implicit none
  character(kind=C_CHAR), pointer, intent(out) :: str(:)
  type(C_PTR) :: str_c

  str_c = tixiGetVersion_c()
  call c_f_stringptr(str_c,str)

end subroutine

! commented out for gfortran <= 4.3 compatibility
!function tixiGetPrintMsgFunc() result(fnc)
!  use, intrinsic :: iso_c_binding
!  implicit none
!  procedure(TixiPrintMsgFnc), pointer :: fnc
!  type(C_FUNPTR) :: fnc_c
!  type(C_PTR) :: fnc_c_
!
!  fnc_c_ = tixiGetPrintMsgFunc_c()
!  ! ugly conversion from C_PTR to C_FUNPTR
!  fnc_c = transfer(fnc_c_, fnc_c)
!  call c_f_procpointer(fnc_c,fnc)
!end function
!
!
!function tixiSetPrintMsgFunc(fnc) result(ret)
!  use, intrinsic :: iso_c_binding
!  implicit none
!  procedure(TixiPrintMsgFnc), pointer, intent(in) :: fnc
!  integer(kind=C_INT) :: ret
!  type(C_FUNPTR) :: fnc_c
!  type(C_PTR) :: fnc_c_
!
!  fnc_c = c_funloc(fnc)
!  ! ugly conversion from C_FUNPTR to C_PTR
!  fnc_c_ = transfer(fnc_c, fnc_c_)
!  ret = tixiSetPrintMsgFunc_c(fnc_c_)
!end function
'''

blacklist = ['tixiGetPrintMsgFunc','tixiSetPrintMsgFunc','tixiGetVersion','tixiGetArrayElementNames']

if __name__ == '__main__':
    # parse the file  
    parser = CP.CHeaderFileParser()
    
    # set the handle string that the parser can identify the handles
    parser.handle_str = 'TixiDocumentHandle'
    parser.returncode_str  ='ReturnCode'
    parser.typedefs = {'TixiPrintMsgFnc': 'void*'}
    parser.parse_header_file(tixipath + '/src/tixi.h')
    
    # create the wrapper
    fg = FG.Fortran03Generator()
    fg.license = apache
    fg.userdeclarations = userdeclarations
    fg.userfunctions = userfunctions
    fg.blacklist = blacklist
    
    print('Creating fortran03 interface... ', end=' ')
    wrapper = fg.create_wrapper(parser)
    print('done')
    
    # write file
    filename = 'tixi3.f90'
    print('Write tixi Fortran 2003 interface to file "%s" ... ' % filename, end=' ') 
    fop = open(filename, 'w')
    fop.write(wrapper)
    print('done')
    
    
