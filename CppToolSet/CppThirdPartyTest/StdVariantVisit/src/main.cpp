#include <variant>	// cpp 17
#include <format>
#include <iostream>
#include <vector>

struct Triangle
{
	int a;
	int b;
	int c;
	
	void Draw() const
	{
		std::cout << "Triangle" << std::endl;
	}
};

struct Circle
{
	int a;
	int b;
	
	void Draw() const
	{
		std::cout << "Circle" << std::endl;
	}
};

struct DrawableVisitDraw
{
	void operator() (const Triangle& rhs) const
	{
		rhs.Draw();
	}

	void operator() (const Circle& rhs) const
	{
		rhs.Draw();
	}
};

int main()
{
	using Drawable = std::variant<Triangle, Circle>;

	Drawable drawableThing = Circle {};

	// variant size = union size + 4B identifier
	std::cout << std::format("Size = {}", sizeof(Drawable)) << std::endl;
	std::cout << std::endl;

	// std::get will throw a exception if fail
	auto* pTriangle = std::get_if<Triangle>(&drawableThing);
	if (pTriangle != nullptr)
		pTriangle->Draw();

	auto* pCircle = std::get_if<Circle>(&drawableThing);
	if (pCircle != nullptr)
		pCircle->Draw();

	std::cout << std::endl;

	// visit
	std::vector<Drawable> drawList {Triangle{}, Circle{}, Triangle{}};

	// using functor structure
	for (const auto &item : drawList)
		std::visit(DrawableVisitDraw{}, item);

	// using auto lambda
	auto DrawVisitor = [](const auto &t)
	{
		t.Draw();
	};
	
	for (const auto &item : drawList)
		std::visit(DrawVisitor, item);

	return 0;
}