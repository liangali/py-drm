import sys 
sys.path.append('../build')
import pydrm

pydrm.open()
for i in range(10):
    pydrm.create_gem(4*1024*1024)
pydrm.close()

print('done')