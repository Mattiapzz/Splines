# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import sys, os, re, subprocess

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
current_file_dir = os.path.normpath(os.path.join(os.path.abspath(__file__), ".."))
project_root_dir = os.path.normpath(os.path.join(current_file_dir, "..", ".."))

# -- Project information -----------------------------------------------------

project = 'Splines'
copyright = '2021, Enrico Bertolazzi'
author = 'Enrico Bertolazzi'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
  #'sphinx.ext.autodoc',
  #'sphinx.ext.doctest',
  #'sphinx.ext.mathjax',
  #'sphinx.ext.viewcode',
  #'sphinx.ext.imgmath',
  #'sphinx.ext.todo',
  #'sphinx.ext.ifconfig',
  'sphinx_rtd_theme',
  'breathe',
  'exhale'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'logo_only': False,
    'navigation_depth': 4,
    'includehidden': True,
    'display_version': True,
}
html_css_files = ["http://netdna.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css"]

# The name of the Pygments (syntax highlighting) style to use.
# pygments_style     = 'sphinx'
# highlight_language = 'c++'
# primary_domain     = 'cpp'

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

def specificationsForKind(kind):
    if kind == "class" or kind == "struct":
        return [
          ":members:",
          ":protected-members:",
          ":private-members:",
          ":undoc-members:"
        ]
    else:
        return []

breathe_projects = {
    "Splines":"../xml/",
}

from exhale import utils

breathe_default_project = "Splines"
exhale_args = {
    "containmentFolder":     "./splines",
    "rootFileName":          "splines.rst",
    "rootFileTitle":         "Splines C++ API",
    "doxygenStripFromPath":  "..",
    # Suggested optional arguments
    "createTreeView":        True,
    "exhaleExecutesDoxygen": True,
    "exhaleDoxygenStdin":    f"""
      INPUT =  {os.path.join(project_root_dir, "src")}
      ENABLE_PREPROCESSING = YES
      MACRO_EXPANSION = YES
      EXTRACT_ALL = YES
      FILE_PATTERNS = *.hxx *.hh *.hpp *.h
    """,
    "customSpecificationsMapping": utils.makeCustomSpecificationsMapping(specificationsForKind)
}


