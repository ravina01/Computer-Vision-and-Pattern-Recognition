"""
Sequence Number : 1
Ravina Lad
Date : 11th March 2022
CS 5330 Computer Vision
Spring 2021

This Python file includes

- Task 1 : Build and train a network to recognize digits
- Task A : Get the MNIST digit data set
- Task B : Make your network code repeatable
- Task C : Build a network model
- Task D : Train the model
- Task E : Save the network to a file

- Task 4 : Design your own experiment
- Task A : Develop a plan
"""
import sys
import torch
import torchvision
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import matplotlib.pyplot as plt
import cv2
import numpy as np
import torchvision.transforms as transforms
from torch.utils.data import DataLoader, Dataset

#Netwrok Defination
class MyNetwork(nn.Module):
    def __init__(self):
        super(MyNetwork, self).__init__()
        self.conv1 = nn.Conv2d(1, 10, kernel_size=5)
        self.conv2 = nn.Conv2d(10, 20, kernel_size=5)
        self.conv2_drop = nn.Dropout2d()
        self.fc1 = nn.Linear(320, 50)
        self.fc2 = nn.Linear(50, 10)

    def forward(self, x):
        x = F.relu(F.max_pool2d(self.conv1(x), 2))
        x = F.relu(F.max_pool2d(self.conv2_drop(self.conv2(x)), 2))
        x = x.view(-1, 320)
        x = F.relu(self.fc1(x))
        x = F.dropout(x, training=self.training)
        x = self.fc2(x)
        return F.log_softmax(x)

#Modified Network Defination
class UpdatedNetwork(nn.Module):
    def __init__(self):
        super(UpdatedNetwork, self).__init__()
        self.conv1 = nn.Conv2d(1, 15, kernel_size=7)
        self.conv2 = nn.Conv2d(15, 30, kernel_size=7)
        #self.conv3 = nn.Conv2d(20, 30, kernel_size=3)
        self.conv2_drop = nn.Dropout2d()
        #self.conv3_drop = nn.Dropout2d()
        self.fc1 = nn.Linear(750, 50)
        self.fc2 = nn.Linear(50, 10)

    def forward(self, x):
        x = F.relu(F.max_pool2d(self.conv1(x), 2))
        x = F.relu(F.max_pool2d(self.conv2_drop(self.conv2(x)), 2))
        #x = F.relu(F.max_pool2d(self.conv3_drop(self.conv3(x)), 2))
        x = x.view(-1, 750)
        x = F.relu(self.fc1(x))
        x = F.dropout(x, training=self.training)
        x = self.fc2(x)
        return F.log_softmax(x)



#Training Network Defination
def train(epoch, network, optimizer, train_loader, train_losses, train_counter, log_interval):
  network.train()
  for batch_idx, (data, target) in enumerate(train_loader):
    optimizer.zero_grad()
    output = network(data)
    loss = F.nll_loss(output, target)
    loss.backward()
    optimizer.step()
    if batch_idx % log_interval == 0:
      print('Train Epoch: {} [{}/{} ({:.0f}%)]\tLoss: {:.6f}'.format(
        epoch, batch_idx * len(data), len(train_loader.dataset),
        100. * batch_idx / len(train_loader), loss.item()))
      train_losses.append(loss.item())
      train_counter.append(
        (batch_idx*64) + ((epoch-1)*len(train_loader.dataset)))
      torch.save(network.state_dict(), '/home/ravina/Desktop/OpenCV/MNIST/modelFiles/model.pth')
      torch.save(optimizer.state_dict(), '/home/ravina/Desktop/OpenCV/MNIST/modelFiles/optimizer.pth')

      #Uncomment This to run task 4 
      #torch.save(network.state_dict(), '/home/ravina/Desktop/OpenCV/MNIST/modelFiles/updated_model.pth')
      #torch.save(optimizer.state_dict(), '/home/ravina/Desktop/OpenCV/MNIST/modelFiles/updated_optimizer.pth')

#Testing Network defination
def test(network, test_loader, test_losses):
  network.eval()
  test_loss = 0
  correct = 0
  with torch.no_grad():
    for data, target in test_loader:
      output = network(data)
      test_loss += F.nll_loss(output, target, size_average=False).item()
      pred = output.data.max(1, keepdim=True)[1]
      correct += pred.eq(target.data.view_as(pred)).sum()
  test_loss /= len(test_loader.dataset)
  test_losses.append(test_loss)
  print('\nTest set: Avg. loss: {:.4f}, Accuracy: {}/{} ({:.0f}%)\n'.format(test_loss, correct, 
                                                                            len(test_loader.dataset),
                                                                            100. * correct / len(test_loader.dataset))) 


# main function to run process of loading MNIST data set

def main(argv):

    #repeatable newtwork
    random_seed = 42
    torch.backends.cudnn.enabled = False
    torch.manual_seed(random_seed)

    #declaration of all Hyperparameters used
    n_epochs = 5
    batch_size_train = 64
    batch_size_test = 1000
    learning_rate = 0.01
    momentum = 0.5
    log_interval = 10

    #Data loader 
    train_loader = torch.utils.data.DataLoader(
                  torchvision.datasets.MNIST('mnist', train=True, download=True,
                             transform=torchvision.transforms.Compose([
                               #transforms.Resize([38,38]), Uncomment this to run Task 4
                                                                      torchvision.transforms.ToTensor(),
                                                                      torchvision.transforms.Normalize(
                                                                      (0.1307,), (0.3081,)) ])),
    batch_size=batch_size_train, shuffle=True)

    test_loader = torch.utils.data.DataLoader(
                  torchvision.datasets.MNIST('mnist', train=False, download=True,
                             transform=torchvision.transforms.Compose([
                               #transforms.Resize([38,38]), uncomment this to run task 4
                                                                      torchvision.transforms.ToTensor(),
                                                                      torchvision.transforms.Normalize(
                                                                      (0.1307,), (0.3081,)) ])),
    batch_size=batch_size_test, shuffle=True)

    print("Examining test data")
    examples = enumerate(test_loader)
    batch_idx, (example_data, example_targets) = next(examples) #get next test data
    print(example_data.shape)

    #plotting first 6 digits from test data set
    fig = plt.figure(figsize=(8,6))
    for i in range(6):
      plt.subplot(2,3,i+1)
      plt.tight_layout()
      plt.imshow(example_data[i][0], cmap='gray', interpolation='none')
      plt.title("Ground Truth: {}".format(example_targets[i]))
      plt.xticks([])
      plt.yticks([])
    fig

    print("Creating a Network")
    network = MyNetwork()
    
    #Uncommnet this to Run Task 4 Modified network
    #network = UpdatedNetwork()

    #setup the optimizer as SGD using the meta-parameters
    optimizer = optim.SGD(network.parameters(), lr=learning_rate,
                      momentum=momentum)
    
    #train the network
    train_losses = []
    train_counter = []
    test_losses = []
    test_counter = [i*len(train_loader.dataset) for i in range(n_epochs + 1)]
    accuracies = []

    #check the performance of the network
    test(network, test_loader, test_losses)

    #Looping over number of epoch
    for epoch in range(1,n_epochs+1):
      train(epoch, network, optimizer, train_loader, train_losses, train_counter, log_interval)
      test(network, test_loader, test_losses)

    #Evaluating Model's Performance
    fig = plt.figure()
    plt.plot(train_counter, train_losses, color='blue')
    plt.scatter(test_counter, test_losses, color='red')
    plt.legend(['Train Loss', 'Test Loss'], loc='upper right')
    plt.xlabel('number of training examples seen')
    plt.ylabel('negative log likelihood loss')
    fig

    with torch.no_grad():
      output = network(example_data)
    
    #Prediction of digits from examples data
    fig = plt.figure()
    for i in range(9):
      plt.subplot(3,3,i+1)
      plt.tight_layout()
      plt.imshow(example_data[i][0], cmap='gray', interpolation='none')
      plt.title("Prediction: {}".format(
        output.data.max(1, keepdim=True)[1][i].item()))
      plt.xticks([])
      plt.yticks([])
    fig

    return
if __name__ == "__main__":
    main(sys.argv)