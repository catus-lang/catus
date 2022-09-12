Dir.chdir(ARGV[0]) do
  Dir["**/*.hpp", "**/*.cpp", "**/*.cc", "**/*.h", "**/*.c"].each do |file|
    puts file
  end  
end
