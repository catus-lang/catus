Dir.chdir(ARGV[0]) do
  Dir["**/*.hpp", "**/*.cpp", "**/*.h", "**/*.c"].each do |file|
    puts file
  end  
end
