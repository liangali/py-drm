import sys 
sys.path.append('../build')
import pydrm

pydrm.open()
engines = pydrm.query_engine()
for e in engines:
    print(e, engines[e])
pydrm.close()

print('done')