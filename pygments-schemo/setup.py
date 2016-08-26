#!/usr/bin/python

from setuptools import setup, find_packages

setup(
    name='pygments-schemo',
    description='Pygments lexer for ScheMo programs.',
    #~ long_description=open('README.md').read(),
    keywords='pygments schemo cpp c++ lexer',

    packages=find_packages(),
    install_requires=['pygments >= 1.4'],

    entry_points='''[pygments.lexers]
                    schemo=pygments_schemo:ScheMoLexer''',

    classifiers=[
        'Development Status :: 1 - Alpha',
        'Environment :: Plugins',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ],
)
