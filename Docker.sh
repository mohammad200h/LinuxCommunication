USER=mamad 
UID=1000
GID=1000

echo $USER
# echo $UID
# echo $GID

echo "installing wget...."
echo "Y"| sudo apt-get install wget

cd DockerFile

echo "buiding nvidia_linux_communication..."
sudo docker build --build-arg UNAME=$USER --build-arg UID=$UID --build-arg GID=$GID -t nvidia_linux_communication .

echo "Image has been created!"

