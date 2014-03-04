#ifndef Clouses_h__
#define Clouses_h__
#include "mockutils/Macros.h"

#include <functional>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <stdexcept>

#include "mockutils/traits.h"
#include "mockutils/DefaultValue.hpp"
#include "fakeit/Exceptions.h"
#include "fakeit/ActualInvocation.h"

namespace fakeit {

struct MethodVerificationProgress {

	MethodVerificationProgress() {
	}

	virtual ~MethodVerificationProgress() THROWS {
	}

	void Never() {
		Times(0);
	}

	virtual void Once() {
		Times(1);
	}

	virtual void Twice() {
		Times(2);
	}

	virtual void AtLeastOnce() {
		verifyInvocations(-1);
	}

	virtual void Times(const int times) {
		if (times < 0) {
			throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
		}
		verifyInvocations(times);
	}

	virtual void AtLeast(const int times) {
		if (times < 0) {
			throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
		}
		verifyInvocations(-times);
	}

protected:
	virtual void verifyInvocations(const int times) = 0;
private:
	MethodVerificationProgress & operator=(const MethodVerificationProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct FirstFunctionStubbingProgress {

	virtual ~FirstFunctionStubbingProgress() THROWS {
	}

	FirstFunctionStubbingProgress<R, arglist...>&
	Return(const R& r) {
		return Do([&r](const arglist&...)->R {return r;});
	}

	void AlwaysReturn(const R& r) {
		return AlwaysDo([&r](const arglist&...)->R {return r;});
	}

	FirstFunctionStubbingProgress<R, arglist...>&
	Return() {
		return Do([](const arglist&...)->R {DefaultValue::value<R>();});
	}

	void AlwaysReturn() {
		return AlwaysDo([](const arglist&...)->R {DefaultValue::value<R>();});
	}

	template<typename E>
	FirstFunctionStubbingProgress<R, arglist...>& Throw(const E& e) {
		return Do([e](const arglist&...)->R {throw e;});
	}

	template<typename E>
	void AlwaysThrow(const E& e) {
		return AlwaysDo([e](const arglist&...)->R {throw e;});
	}

	virtual void operator=(std::function<R(arglist...)> method) {
		AlwaysDo(method);
	}

	virtual auto Do(std::function<R(arglist...)> method) -> FirstFunctionStubbingProgress<R, arglist...>& = 0;

	virtual auto AlwaysDo(std::function<R(arglist...)> method) -> void = 0;

private:
	FirstFunctionStubbingProgress & operator=(const FirstFunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct FirstProcedureStubbingProgress {

	virtual ~FirstProcedureStubbingProgress() THROWS {
	}

	FirstProcedureStubbingProgress<R, arglist...>& Return() {
		return Do([](const arglist&...)->R {return DefaultValue::value<R>();});
	}

	void AlwaysReturn() {
		return AlwaysDo([](const arglist&...)->R {return DefaultValue::value<R>();});
	}

	template<typename E>
	FirstProcedureStubbingProgress<R, arglist...>& Throw(const E e) {
		return Do([e](const arglist&...)->R {throw e;});
	}

	template<typename E>
	void AlwaysThrow(const E e) {
		return AlwaysDo([e](const arglist&...)->R {throw e;});
	}

	virtual void operator=(std::function<R(arglist...)> method) {
		AlwaysDo(method);
	}

	virtual FirstProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;

	virtual auto AlwaysDo(std::function<R(arglist...)> method) -> void = 0;

private:
	FirstProcedureStubbingProgress & operator=(const FirstProcedureStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct FunctionStubbingProgress: public virtual FirstFunctionStubbingProgress<R, arglist...> {

	FunctionStubbingProgress() = default;
	virtual ~FunctionStubbingProgress() {
	}

private:
	FunctionStubbingProgress & operator=(const FunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct ProcedureStubbingProgress: //
public virtual FirstProcedureStubbingProgress<R, arglist...> {

	ProcedureStubbingProgress() = default;
	virtual ~ProcedureStubbingProgress() override {
	}
	;

private:
	ProcedureStubbingProgress & operator=(const ProcedureStubbingProgress & other) = delete;
};

}

#endif // Clouses_h__
