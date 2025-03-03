auto SMP::readRAM(uint16 address) -> uint8 {
  if(address >= 0xffc0 && io.iplromEnable) return iplrom[address & 0x3f];
  if(io.ramDisable) return 0x5a;  //0xff on mini-SNES
  return dsp.apuram[address];
}

auto SMP::writeRAM(uint16 address, uint8 data) -> void {
  //writes to $ffc0-$ffff always go to apuram, even if the iplrom is enabled
  if(io.ramWritable && !io.ramDisable) dsp.apuram[address] = data;
}

auto SMP::idle() -> void {
  waitIdle();
}

auto SMP::read(uint16 address) -> uint8 {
  uint8 data = readRAM(address);
  if((address & 0xfff0) == 0x00f0) data = readIO(address);
  wait(address);
  return data;
}

auto SMP::write(uint16 address, uint8 data) -> void {
  wait(address);
  writeRAM(address, data);  //even IO writes affect underlying RAM
  if((address & 0xfff0) == 0x00f0) writeIO(address, data);
}

auto SMP::readDisassembler(uint16 address) -> uint8 {
  if((address & 0xfff0) == 0x00f0) return 0x00;
  return readRAM(address);
}
