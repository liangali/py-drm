import sys 
sys.path.append('../build')
import pydrm

pydrm.open()
engines = pydrm.query_engine()
print(engines)
pydrm.close()

print('done')