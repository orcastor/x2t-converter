#ifndef TYPESELEMENTS_H
#define TYPESELEMENTS_H
namespace StarMath
{
enum class TypeElement{
	undefine,
	//global
	String,
	BinOperator,
	SetOperations,
	Operator,
	Bracket,
	//BracketEnd,
	UnarSign,
	Attribute,
	SpecialSymbol,
	Function,
	Operation,
	Index,
	Matrix,
	Connection,
	Empty,
	//binoop
	cdot,
	times,
	over,
	plus,
	minus,
	frac,
	div,
	multipl,
	division,
	oplus,
	ominus,
	odot,
	otimes,
	odivide,
	circ,
	wideslash,
	widebslash,
	//op
	lim,
	sum,
	liminf,
	limsup,
	prod,
	coprod,
	Int,
	iint,
	iiint,
	lint,
	llint,
	lllint,
	//brace
	brace,
	round,
	square,
	ldbracket,
	lbrace,
	langle,
	lceil,
	lfloor,
	lline,
	ldline,
	//attribute
	ital,
	bold,
	//top element
	acute,
	breve,
	dot,
	dddot,
	vec,
	tilde,
	check,
	grave,
	circle,
	ddot,
	bar,
	harpoon,
	hat,
	widevec,
	widetilde,
	overline,
	overstrike,
	wideharpoon,
	widehat,
	underline,//top elements
	color,
	black,
	green,
	aqua,
	yellow,
	lime,
	navy,
	purple,
	teal,
	blue,
	red,
	fuchsia,
	gray,
	maroon,
	olive,
	silver,
	coral,
	midnightblue,
	crimson,
	violet,
	orange,
	seagreen,
	hotpink,
	orangered,
	indigo,
	lavender,
	//color(without rgb and hex)
	mline,
	grid,
	dlgrid,
	//setopetions
	intersection,
	Union,
	setminus,
	setquoyient,
	subseteq,
	subset,
	supset,
	supseteq,
	nsubset,
	nsupseteq,
	nsupset,
	nsubseteq,
	in,
	notin,
	owns,
	//connection
	approx,
	sim,
	simeq,
	equiv,
	prop,
	parallel,
	ortho,
	divides,
	ndivides,
	toward,
	transl,
	transr,
	def,
	equals,
	notequals,
	learrow,
	learrowequals,
	leslant,
	riarrow,
	riarrowequals,
	geslant,
	dllearrow,
	dlriarrow,
	prec,
	succ,
	preccurlyeq,
	succcurlyeq,
	precsim,
	succsim,
	nprec,
	nsucc,
	dlarrow,
	dlrarrow,
	drarrow,
	//
	emptyset,
	aleph,
	setN,
	setZ,
	setQ,
	setR,
	setC,
	infinity,
	partial,
	nabla,
	exists,
	notexists,
	forall,
	hbar,
	lambdabar,
	Re,
	Im,
	wp,
	laplace,
	fourier,
	backepsilon,
	//function
	abs,
	fact,
	sqrt,
	nroot,//nroot?
	sin,
	cos,
	tan,
	cot,
	sinh,
	cosh,
	tanh,
	coth,
	arcsin,
	arccos,
	arctan,
	arccot,
	arsinh,
	arcosh,
	artanh,
	arcoth,
	ln,
	exp,
	log,
	//index
	upper,
	lower,
	lsup,
	lsub,
	csup,
	csub,
	//
	binom,
	stack,
	matrix,
	//bracket close
	rwbrace,
	rbrace,
	rround,
	rsquare,
	rdbracket,
	rangle,
	rceil,
	rfloor,
	rline,
	rdline,
	right,
	//op
	from,
	to,
};
}
#endif // TYPESELEMENTS_H
