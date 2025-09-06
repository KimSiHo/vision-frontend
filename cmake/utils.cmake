include_guard(GLOBAL)

function(find_or_pkg OUT_TARGET CONFIG_PKG CONFIG_TARGET PKG_NAME)
  find_package(${CONFIG_PKG} CONFIG QUIET)
  if(TARGET ${CONFIG_TARGET})
    set(${OUT_TARGET} ${CONFIG_TARGET} PARENT_SCOPE)
  else()
    find_package(PkgConfig REQUIRED)
    # PkgConfig::<ALIAS> 이름을 CONFIG_PKG로 맞춰둠
    pkg_check_modules(${CONFIG_PKG} REQUIRED IMPORTED_TARGET ${PKG_NAME})
    set(${OUT_TARGET} PkgConfig::${CONFIG_PKG} PARENT_SCOPE)
  endif()
endfunction()