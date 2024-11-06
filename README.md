# secret-rwx
Method to make an RWX region without an actual RWX region. Typically when you want to run shellcode, you will use a RWX region or make a RW and switch to RX. Both are sigged very very hard.

Introducing this novel technique - you have a RW and a RX region, backed by the same physical memory, but at differing virtual addresses. This means any changes written to the RW page, is reflected in the RX page. So this becomes essentially a RWX region, without the signatures.

![image](https://github.com/user-attachments/assets/cc1c298c-6e92-47e4-9e02-f610193aa0b8)

Also as a nice bonus, the RW and RX pages created is considered mapped memory, so you get less sigs.
