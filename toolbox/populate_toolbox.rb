#!/usr/bin/ruby -w

require 'fileutils'

FileUtils.rm_rf "src"
Dir.glob("bin/*.mex*").each { |file| File.delete(file)}

FileUtils.cp_r  "../src/.", "./src";
FileUtils.cp_r  "../submodules/quarticRootsFlocke/src/.",   "./src";
FileUtils.cp_r  "../submodules/Utils/src/.",                "./src";
FileUtils.cp_r  "../submodules/GenericContainer/src/.",     "./src";
FileUtils.cp_r  "../submodules/GenericContainer/include/.", "./src";
FileUtils.rm_rf "./src/Eigen";

#lst = Dir["../doc/*"]
#lst.each do |filename|
#  FileUtils.cp filename, "./doc/" + File.basename(filename);
#end

FileUtils.cp "../license.txt", "license.txt"

FileUtils.cp "../submodules/GenericContainer/src_matlab_interface/GenericContainerMatlabInterface.cc",
             "./src/GenericContainerMatlabInterface.cc"

FileUtils.cp "../license.txt", "license.txt"
