from building import *
import rtconfig

cwd     = GetCurrentDir()
src     = Glob('*.c')
CPPPATH = [cwd]
LOCAL_CCFLAGS = ''


group = DefineGroup('tx_to_rt', src, depend = ['PKG_USING_THREADX_TO_RT'], CPPPATH = CPPPATH, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
