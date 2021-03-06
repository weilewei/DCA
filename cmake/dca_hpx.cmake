################################################################################
# Find HPX
################################################################################

set(DCA_HAVE_HPX FALSE)

find_package(HPX)
if (HPX_FOUND)
  set(DCA_HAVE_HPX TRUE)
  dca_add_haves_define(DCA_HAVE_HPX)
  include_directories(${HPX_INCLUDE_DIRS})
endif()
