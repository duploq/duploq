cp ./SPECS/duploq.spec ~/rpmbuild/SPECS
cd ../../../
sudo make install
cd ~/rpmbuild/SPECS
rpmbuild -bb duploq.spec

