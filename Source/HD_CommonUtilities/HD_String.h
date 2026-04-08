#pragma once

class HD_String
{
public:
	HD_String();
	HD_String(const char* aString);
	HD_String(const HD_String& aString);
	HD_String(HD_String&& aString);
	~HD_String();

	const char* GetBuffer() const;
	int GetLength() const;
	char GetCharAt(int aIndex) const;

	void Append(const char* aString);
	void Append(const HD_String& aString);

	HD_String operator+(const char* aString) const;
	HD_String operator+(const HD_String& aString) const;

	HD_String& operator=(const char* aString);
	HD_String& operator=(const HD_String& aString);
	HD_String& operator=(HD_String&& aString);

	bool operator==(const char* aString) const;
	bool operator==(const HD_String& aString) const;
	bool operator!=(const char* aString) const;
	bool operator!=(const HD_String& aString) const;

private:
	static constexpr float ourGrowFactor = 1.5f;

	void CheckLengthAndGrowIfNecessary(int anAdditionalLength);
	void Grow(int aNewCapacity);

	char* myData;
	int myLength;
	int myCapacity;
};

// Global operator for doing <string literal> + aString.
HD_String operator+(const char* aString1, const HD_String& aString2);
