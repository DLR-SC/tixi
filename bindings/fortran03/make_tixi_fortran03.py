# -*-# coding: utf-8 -*-
"""
Created on Wed Sep 16 10:58:02 2015

@author: Melven Roehrig-Zoellner <Melven.Roehrig-Zoellner@DLR.de>
"""

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
function tixiGetVersion() result(res)
  use, intrinsic :: iso_c_binding
  implicit none
  interface
    function tixiGetVersion_c() result(ret) bind(C,name='tixiGetVersion')
      use, intrinsic :: iso_c_binding
      type(C_PTR) :: ret
    end function
  end interface
  character(kind=C_CHAR,len=200) :: res
  character(kind=C_CHAR),pointer :: str(:)
  type(C_PTR) :: str_c
  integer :: i

  str_c = tixiGetVersion_c()
  res = ' '
  do i = 1, 200
    call c_f_pointer(str_c,str,(/i/))
    if( str(i) .eq. C_NULL_CHAR ) exit
    res(i:i) = str(i)
  end do

end function


function tixiGetPrintMsgFunc() result(fnc)
  use, intrinsic :: iso_c_binding
  implicit none
  interface
    function tixiGetPrintMsgFunc_c() result(ret) bind(C,name='tixiGetPrintMsgFunc')
      use, intrinsic :: iso_c_binding
      type(C_FUNPTR) :: ret
    end function
  end interface
  procedure(TixiPrintMsgFnc), pointer :: fnc
  type(C_FUNPTR) :: fnc_c

  fnc_c = tixiGetPrintMsgFunc_c()
  call c_f_procpointer(fnc_c,fnc)
end function


function tixiSetPrintMsgFunc(fnc) result(ret)
  use, intrinsic :: iso_c_binding
  implicit none
  interface
    function tixiSetPrintMsgFunc_c(func) result(ret) bind(C,name='tixiSetPrintMsgFunc')
      use, intrinsic :: iso_c_binding
      type(C_FUNPTR), intent(in) :: func
      integer(kind=C_INT) :: ret
    end function
  end interface
  procedure(TixiPrintMsgFnc), pointer, intent(in) :: fnc
  integer(kind=C_INT) :: ret
  type(C_FUNPTR) :: fnc_c

  fnc_c = c_funloc(fnc)
  ret = tixiSetPrintMsgFunc_c(fnc_c)
end function
'''

#postconstr = '''
#self.version = self.getVersion()
#'''


blacklist = ['tixiGetVersion','tixiGetPrintMsgFunc','tixiSetPrintMsgFunc']

if __name__ == '__main__':
    # parse the file
    
    #ann =   CP.Annotation('#annotate out: 3, 4A(3)')    
    
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
    
    print 'Creating python interface... ',
    wrapper = fg.create_wrapper(parser)
    print 'done'
    
    # write file
    filename = 'tixi.f90'
    print 'Write tixi Fortran 2003 interface to file "%s" ... ' % filename, 
    fop = open(filename, 'w')
    fop.write(wrapper)
    print 'done'
    
    
