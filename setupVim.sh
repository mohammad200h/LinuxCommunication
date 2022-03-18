#https://www.youtube.com/watch?v=QB9V__3VO2s
#install neovim
sudo apt-get install python3.6-dev
sudo add-apt-repository ppa:neovim-ppa/unstable
sudo apt-get update
sudo apt-get install neovim


# mkdir ~/.config
#Download latest setting from dropobox
echo "Downloading latest nvim form dropbox...."
wget https://www.dropbox.com/s/q8z6j37nb6k0nik/nvim.zip?dl=0

echo "renaming downloaded files...."
mv ./nvim.zip?dl=0 nvim.zip

echo "unziping downloaded files..."
echo "y" | unzip nvim.zip


echo "moving the setting to .config"

sudo mv ./nvim  ~/.config/