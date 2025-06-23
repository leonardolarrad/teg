import teg.varint;

int main() {
	return !(teg::uleb128::encoded_size(0xFFFFFFFFFFFFFFFFULL) == 10);
}