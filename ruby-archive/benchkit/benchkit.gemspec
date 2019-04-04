# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'benchkit/version'

Gem::Specification.new do |spec|
  spec.name          = 'benchkit'
  spec.version       = Benchkit::VERSION
  spec.authors       = ['Takashi Kokubun']
  spec.email         = ['takashikkbn@gmail.com']

  spec.summary       = %q{Old implementation of benchmark_driver}
  spec.description   = %q{Old implementation of benchmark_driver}
  spec.homepage      = 'https://github.com/k0kubun/benchmark_driver'
  spec.license       = 'MIT'

  spec.files         = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']
  spec.required_ruby_version = '>= 2.3.0'

  spec.add_development_dependency 'bundler'
  spec.add_development_dependency 'rake'
end
