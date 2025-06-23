import teg.serialization;

int main() {
	teg::byte_array buffer = {};
	
	teg::i32 i0 = 99;
	teg::serialize(buffer, i0).or_throw();

	teg::i32 i1 = 0;
	teg::deserialize(buffer, i1).or_throw();

	return !(i0 == i1);
}