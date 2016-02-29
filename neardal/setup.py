from distutils.core import setup, Extension

moduleNDEF = Extension('ndef',
                       define_macros=[('MAJOR_VERSION', '1'),
                                      ('MINOR_VERSION', '0')],
                       include_dirs=['/usr/local/include'],
                       libraries=['neardal'],
                       library_dirs=['/usr/local/lib'],
                       sources=['neardal.c'])

setup(name='NeardalPy',
      version '1.0',
      description='libneardal binding for python',
      author='Louis Desportes',
      author_email='louis@akkes.fr',
      url='https://github.com/akkes/Daladin',
      long_description='''
A libneardal binding for python.
Allow read/write of NFC NDEF and cards
''',
      ext_modules=[moduleNDEF])
