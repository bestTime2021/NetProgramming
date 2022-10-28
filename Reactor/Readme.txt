how can we include like muduo did
1,	we should create a library, you can see library/base/CMakeLists.txt
	and you will find add_library(library_base source_file)
	which means you create a library named library_base, and add a implement to it

2,	in order to get use of library "library_base",
	we first add_directory(library/base), which means we add a source file
	then, include_directories(${PROJECT_SOURCE_DIR}) is also a essential step. by doing this, other source file can find include file correctly.

3,	now we can use library as we want.(eg: hello/hh.cc use function1)
