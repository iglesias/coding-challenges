sh setup
pushd panim
cc nob.c -o nob
./nob
popd
sh compile
ln -s src/main.cpp main.cpp
cd panim
./build/panim ../libtree.so
# exit with q
# pause with whitespace
# increase speed with .
# decrease with ,
# next frame with the right key >
# prev frame with the left key <
# reload (generating a new tree) with h
# restart traversal in current tree with r
# i and s toggle detail and source views
