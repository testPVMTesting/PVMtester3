require 'find'


Find.find('.') do |entry|
    if /\.(cpp|h)$/ =~ entry
        contents = IO.readlines entry
        if %r{// PRIVATE} =~ contents[0]
            STDERR.puts "#{entry} should not be here"
        end
    end
end