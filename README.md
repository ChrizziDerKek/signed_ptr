# signed_ptr
Pointer class that can be used to verify safe pointer modifications. This is more of a PoC than something that would be actually useful.

## How it works

[Virtual addresses are in the range of 0 to 0x7FFFFFFFFFFF in x64 windows](https://learn.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/virtual-address-spaces)

If we look closely at the maximum value 0x7FFFFFFFFFFF, we notice that it doesn't use the full 64bit that we have in an 8 byte integer. In fact, actually 17 bits are unused which gives us plenty of space to play with. So what I did was calculating a hash using a modified fnv1a hash algorithm and storing that hash in 16 of the 17 unused bits. 1 bit is used as an indicator that the pointer contains a hash. This hash can then be used to check if the current pointer value is still valid or if it was modified in an illegal way.

## Example

The pointer ``0x7FFE9A168B70`` looks like this in binary ``0b0000000000000000011111111111111010011010000101101000101101110000``.

Notice the unused bits at the start, where we can store our hash. After getting the raw pointer, we calculate a fnv1a 32 bit hash from it, which would be ``0x6F759D13``.

But since we only have 17 bits of free space and the hash would require 32, we just split the hash into the upper and lower parts (``0x6F75`` and ``0x9D13``) and xor the 2 values to get a 16 bit value (``0xF266``). That value fits perfectly into 16 of our 17 bits.

In the last step, we write the hash into the pointer and set the last remaining bit to 1 to indicate that a hash is present in the pointer.

The resulting value will look like this in binary ``0b1111001001100110111111111111111010011010000101101000101101110000`` or in hexadecimal ``0xF266FFFE9A168B70``.

## How to use

You can use t

First, download and include the ``signed_ptr.h`` file into your project. Then you can use the ``signed_ptr`` datatype like you would use a smart pointer, except that this one isn't that smart. You can either create such a pointer by giving it an existing pointer like this:
```cpp
signed_ptr<decltype(MessageBoxA)> messagebox_ptr = MessageBoxA;
(*messagebox_ptr)(0, "Hello World!", "", 0);
```
or you can use ``make_signed`` to construct a new object:
```cpp
class sample
{
public:
  int something;
  sample(int s) { something = s; }
};
signed_ptr<sample> sample_ptr = make_signed<sample>(1337);
int sample_ptr_value = sample_ptr->something;
```
You can also access any attributes of the object as usual with the ``->`` operator or by dereferencing the sample ptr.
