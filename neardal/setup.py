from distutils.core import setup, Extension

module_neardal = Extension('neardal',
                           define_macros=[('MAJOR_VERSION', '1'),
                                          ('MINOR_VERSION', '0')],
                           include_dirs=['/usr/local/include',
                                         '/usr/include/glib-2.0',
                                         '/usr/lib/arm-linux-gnueabihf/glib-2.0/include',
                                         '/usr/local/include/neardal'],
                           libraries=['neardal', 'glib'],
                           library_dirs=['/usr/local/lib'],
                           sources=['adaptermodule.c', 'neardal.c'])

setup(name='NeardalPy',
      version='0.1',
      description='libneardal binding for python',
      author='Louis Desportes',
      author_email='louis@akkes.fr',
      url='https://github.com/akkes/Daladin',
      long_description='''
A libneardal binding for python.
Allow read/write of NFC NDEF and cards
''',
      ext_modules=[module_neardal])
