########################################
# BEGIN_COPYRIGHT
#
# This file is part of SciDB.
# Copyright (C) 2008-2014 SciDB, Inc.
#
# SciDB is free software: you can redistribute it and/or modify
# it under the terms of the AFFERO GNU General Public License as published by
# the Free Software Foundation.
#
# SciDB is distributed "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
# INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
# NON-INFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE. See
# the AFFERO GNU General Public License for the complete license terms.
#
# You should have received a copy of the AFFERO GNU General Public License
# along with SciDB.  If not, see <http://www.gnu.org/licenses/agpl-3.0.html>
#
# END_COPYRIGHT
########################################

# shared library for user defined objects
set (bestmatch_src
    plugin.cpp
    BestMatchArray.cpp
    LogicalBestMatch.cpp
    PhysicalBestMatch.cpp
)

add_library(bestmatch SHARED ${bestmatch_src})
extractDebugInfo("${GENERAL_OUTPUT_DIRECTORY}/plugins" "libbestmatch${CMAKE_SHARED_LIBRARY_SUFFIX}" bestmatch)
set_target_properties(bestmatch PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${GENERAL_OUTPUT_DIRECTORY}/plugins)

if (APPLE)
  set_target_properties(bestmatch PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif (APPLE)

