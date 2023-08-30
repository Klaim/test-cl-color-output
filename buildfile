libs =
import libs += libboost-process%lib{boost_process} libboost-algorithm%lib{boost_algorithm} libbutl%lib{butl}

./: exe{forward} doc{README.md} manifest

exe{forward}: {hxx ixx txx cxx}{*} $libs

cxx.poptions =+ "-I$out_root" "-I$src_root"
