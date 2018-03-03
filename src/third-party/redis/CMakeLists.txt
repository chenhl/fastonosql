CMAKE_MINIMUM_REQUIRED(VERSION 3.6.3) # OPENSSL_USE_STATIC_LIBS
PROJECT(hiredis)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/deps/hiredis ${CMAKE_CURRENT_LIST_DIR}/src ../sds)

SET(HEADERS_HIREDIS
  deps/hiredis/fmacros.h
  deps/hiredis/read.h
  deps/hiredis/net.h
  deps/hiredis/hiredis.h
)

SET(SOURCES_HIREDIS
  deps/hiredis/net.c
  deps/hiredis/read.c
  deps/hiredis/hiredis.c
)

ADD_LIBRARY(hiredis STATIC ${HEADERS_HIREDIS} ${SOURCES_HIREDIS})
