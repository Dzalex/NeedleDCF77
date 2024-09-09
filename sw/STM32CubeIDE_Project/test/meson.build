project('NeedleDCF77', 'c',
  license: 'MIT',
  default_options: [
    'c_std=c99',
    'warning_level=3',
  ],
  meson_version: '>= 0.49.0'
)

unity_subproject = subproject('unity')
unity_dependency = unity_subproject.get_variable('unity_dep')
unity_gen_runner = unity_subproject.get_variable('gen_test_runner')

src1 = files([
  '../Core/Src/dcf77Decoding.c',
  'TestDCF77Decoding.c'
])

inc = include_directories('../Core/Inc')


test1 = executable('test1',
  sources: [
    src1,
    unity_gen_runner.process('TestDCF77Decoding.c')
  ],
  include_directories: [ inc ],
  dependencies: [ unity_dependency ]
)

test('test1', test1,
  should_fail: true)


