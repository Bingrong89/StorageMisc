import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data import Dataset, DataLoader, Sampler
import time
import sys

from custom_conv import better_compute3 as bc
import custom_dataload as cd

device=torch.device('cpu')

class CustomNet(nn.Module):
    def __init__(self):
        super(CustomNet,self).__init__()
        #pytorch..(in_channels,out_channels,kernel_size,stride=1,padding=0)
    #    self.a1 = nn.Conv2d(3,30,1) #transform 969
     #   self.a2 = nn.Conv2d(30,45,3,3,dilation=2) #customconv 969->645 
        
        self.a3 = nn.Conv2d(3,30,1) #transform 645
        self.a4 = nn.Conv2d(30,45,3,3,dilation=2) #customconv 645->429
        
        self.a5 = nn.Conv2d(3,45,1) #transform 429
        self.a6 = nn.Conv2d(45,68,3,3,dilation=2) #custom 429->285
        
        self.a7 = nn.Conv2d(3,68,1) #transform 285
        self.a8 =nn.Conv2d(68,102,3,3,dilation=2) #285->189
        
        self.a9 = nn.Conv2d(102,153,3,3,dilation=2) #189->125
        
        #normal things...
        self.conv1 = nn.Conv2d(153,153,3) #125->123
        self.a10 = nn.Conv2d(153,230,3,3,dilation=2) #123->81
        self.a11 =nn.Conv2d(230,345,3,3,dilation=2) #81->53
        self.conv2 = nn.Conv2d(345,345,3) #53->51
        self.a12 = nn.Conv2d(345,345,3,3,dilation=2) #51->33  more possibilities ..33->21 21->13
        
        self.a13= nn.Conv2d(345,345,3,3,dilation=2) #33->21
        self.a14 = nn.Conv2d(345,345,3,3,dilation=2) #21->13
        
        self.fc1 = nn.Linear(345*13*13,2048)
        self.fc2 = nn.Linear(2048,10) #10 classes.. 
        
        
    def forward(self,x):
        #try one fwd pass with 1x3x969x969 input
   #     x = F.relu(self.a1(x))
    #    x = bc(x) #this intermediate product can be destroyed immediately to free up memory,backprop still works fine
     #   x = F.relu(self.a2(x))
        print("fwd pass called")
        if x.size()[2] and x.size()[3] == 645:
            x = self.a3(x)
            x = self.a4(bc(x)) #645->429
            x = self.a6(bc(x))
        
        elif x.size()[2] and x.size()[3] == 429:
            x = self.a5(x)
            x = self.a6(bc(x))
            
        elif x.size()[2] and x.size()[3] == 285: 
            x = self.a7(x)
        
        else:
            raise Exception("Input HxW of %f x %f is invalid" %(x.size()[2],x.size()[3]))
        
        x = self.a8(bc(x))     
        x = self.a9(bc(x))      
        x = F.relu(self.conv1(x))
        x = self.a10(bc(x))     
        x = self.a11(bc(x)) # mmmm...       
        x = F.relu(self.conv2(x))        
        x = self.a12(bc(x))        
        x = self.a13(bc(x)) #mmmm...       
        x = self.a14(bc(x))      
        x = F.relu(self.fc1(x.view(x.size()[0],-1)))      
        x = F.relu(self.fc2(x))
       
        return x
		
		
		