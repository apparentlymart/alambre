import sys, os

extensions = ['breathe']
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'

project = u'Alambre'
copyright = u'2012, Martin Atkins'

version = '0.1'
release = '0.1'

pygments_style = 'sphinx'
html_theme = 'default'
html_static_path = ['_static']

breathe_projects = {
    "alambre": "docs/doxygen/xml/",
}
breathe_default_project = "alambre"

breathe_domain_by_extension = {
    "cpp": "cpp",
    "h": "cpp",
}
