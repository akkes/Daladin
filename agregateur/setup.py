try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup

config = {
    'description': 'Aggregator for Daladin',
    'author': 'Louis Desportes',
    'url': 'https://github.com/akkes/Daladin',
    'download_url': 'https://github.com/akkes/Daladin/archive/master.zip',
    'author_email': 'louis@akkes.fr',
    'version': '0.1',
    'install_requires': ['nose', 'requests', 'bs4', 'feedparser'],
    'packages': ['NAME'],
    'scripts': [],
    'name': 'daladin'
}

setup(**config)
