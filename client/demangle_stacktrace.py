import os
import re
import subprocess

def demangle_cpp_names(stacktrace):
    demangled_names = []
    for address in stacktrace:
        try:
            # Use subprocess to call c++filt to demangle the name
            demangled_name = subprocess.check_output(['c++filt', address]).decode('utf-8').strip()
            demangled_names.append(demangled_name)
        except subprocess.CalledProcessError:
            # In case of any error (e.g., invalid address), add the original address
            demangled_names.append(address)

    return demangled_names

def get_addr(bytes_after_base, address_offset):
    address_hex = bytes_after_base[address_offset * 4 : (address_offset + 1) * 4]
    address_str = str(b''.join(address_hex[::-1]), encoding='ascii')
    return int(address_str, 16)

def parse_hexdump(hexdump):
    stacktrace = []
    lines = hexdump.split(b'\n')
    for line in lines:
        parts = line.split()
        if len(parts) < 17:
            continue
        bytes_after_base = parts[1:17]
        for address_offset in range(4):
            address = get_addr(bytes_after_base, address_offset)
            if address != 0:
                stacktrace.append(address)  # Add the address to the list
    return stacktrace

def parse_symbolic_info(sym_info):
    symbol_dict = {}
    lines = sym_info.split('\n')
    for line in lines:
        if line.strip() == '':
            continue
        address, symbol = line.split(' ')
        address_int = int(address, 16)  # Convert hex address to integer
        if address_int >= int('60000000', 16):
            break
        symbol_dict[address_int] = symbol.strip()  # Remove any leading/trailing whitespaces

    return symbol_dict

if __name__ == "__main__":
    client_dir = "client"
    current_dir = os.getcwd()

    if os.path.exists(os.path.join(current_dir, client_dir)):
        os.chdir(client_dir)

    # Replace 'stack.dmp' and 'ds2key.sym' with the actual file paths
    with open('stack.dmp', 'rb') as hex_file:
        hexdump_content = hex_file.read()

    with open('ds2key.sym', 'r') as sym_file:
        sym_content = sym_file.read()

    stacktrace = parse_hexdump(hexdump_content)
    symbol_info = parse_symbolic_info(sym_content)

    demangled_names = []
    addresses = []
    deny_these = {'__irq_vector', '__vectors_start', '__iwram_top', '__itcm_end', '__sbss_start__',  '_stack', '_impure_ptr', '__twl_end__', '_impure_data'}
    for address in stacktrace:
        matched_symbols = [addr for addr in symbol_info if addr <= address and addr != 0] 
        if matched_symbols:
            latest_symbol_address = max(matched_symbols)
            symbol = symbol_info[latest_symbol_address]
            if symbol not in deny_these:
                demangled_names.append(symbol)
                addresses.append(address)

    demangled_stacktrace = demangle_cpp_names(demangled_names)
    for address, func in zip(addresses, demangled_stacktrace):
        print(f'{hex(address)[2:].zfill(8)}: {func}')
