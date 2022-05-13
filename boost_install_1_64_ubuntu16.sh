#https://stackoverflow.com/questions/8430332/uninstall-boost-and-install-another-version

sudo apt-get update
echo "installing wget"
sudo apt-get install wget

echo "removing existing boost library....."
sudo apt-get -y --purge remove libboost-all-dev libboost-doc libboost-dev
sudo rm -f /usr/lib/libboost_*

echo "Installing boost dependecies...."

sudo apt-get -y install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev



echo" going to home folder"
cd ..
echo"Downloading boost_1_64_0...."
wget http://downloads.sourceforge.net/project/boost/boost/1.64.0/boost_1_64_0.tar.gz
echo "Unziping..."
tar -zxvf boost_1_64_0.tar.gz

cd boost_1_64_0
# get the no of cpucores to make faster
cpuCores=`cat /proc/cpuinfo | grep "cpu cores" | uniq | awk '{print $NF}'`
echo "Available CPU cores: "$cpuCores
./bootstrap.sh  # this will generate ./b2
sudo ./b2 --with=all -j $cpuCores install
