project('devkit', 'cpp')

cpp = meson.get_compiler('cpp')

ssl_dep = dependency('openssl', required: true) 

inc = include_directories('lib')
sources = files('lib/Tokens.cpp')

devkit_lib = shared_library('devkit', sources,
  include_directories: inc,
  build_by_default: true,
  dependencies: [ssl_dep] 
)

devkit_dep = declare_dependency(
  include_directories: inc,
  link_with: devkit_lib,
  dependencies: [ssl_dep] 
)