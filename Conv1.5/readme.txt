Code originally created for a 'flexible neural network' that has multiple entry points depending on input image size.
Thus, instead of having to resize input images all to the same size, this architecture provides multiple resize options.
Really just because I am not very comfortable with how we resize images to the same size when they can differ significantly in size.


Spent some time creating a 'convolution kernel' which works at stride =1.5,kernelsize = 3.
Thus the resize options are controlled by this kernel.
Naturally using a normal convolution kernel could also give its own resize options, albeit halving matrix 
feature maps' HxW does provide lesser resize options.

Also created custom collate and sampler in pytorch.
Unfortunately after creating this dataset, I could not find an image dataset that had significantly varying sizes
and are atleast somewhat balanced.
PASCAL VOC images are mostly around one size. Maybe can come back and test this in future.. maybe.
