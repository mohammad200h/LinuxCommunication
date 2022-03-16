FROM nvidia/cudagl:11.2.0-devel-ubuntu20.04
#https://github.com/phusion/baseimage-docker/issues/58
#Telling consoul to shut the fuck up!
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
#****************Setting up user *********************
ARG UNAME=ros
ARG PASS=ros
ARG UID=1000
ARG GID=1000
RUN apt-get update && apt-get install sudo -y
RUN sudo -i
RUN groupadd -g $GID -o $UNAME
RUN sudo useradd -m -u $UID -g $GID -p $(openssl passwd -1 $PASS) -o -s  /bin/bash $UNAME
RUN usermod -aG sudo $UNAME
USER $UNAME 
WORKDIR home/$UNAME/ 


#****************************
RUN echo $PASS | sudo -S apt-get update
# RUN printf '%s\n%s\n%s\n%s\n' ${PASS} 'y' '8' '27'| sudo -S apt-get install software-properties-common

RUN echo $PASS |  sudo -S apt-get install -y software-properties-common
RUN echo $PASS |  sudo -S apt-get install -y build-essential cmake 
#***********************************Installing TF*************************

RUN echo $PASS|sudo -S apt-get update


RUN echo $PASS|sudo -S apt-get update

RUN printf '%s\n%s\n' ${PASS} 'y' |sudo -S apt-get install python3-pip

RUN pip3 install --upgrade pip==20.2.4

#*************************terminator***************
# RUN echo $PASS | sudo -S apt-get install -y software-properties-common

# RUN echo "\u000A" | sudo -S add-apt-repository ppa:gnome-terminator
RUN echo $PASS | sudo -S apt-get update

RUN printf '%s\n%s\n' ${PASS} 'y'|sudo -S apt-get install terminator

#***********************************
sudo apt-get install libjsoncpp-dev 
sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json
#**********************************
CMD /bin/bash